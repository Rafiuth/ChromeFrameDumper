#include "Main.h"

#define AV_CHECK(x) _AV_CHECK(x, #x)
#define AV_CHECK_NULL(x) _AV_CHECK_NULL(x, #x)

void _AV_CHECK(int ret, const char* expr)
{
    if (ret < 0) {
        char errBuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errBuf, sizeof(errBuf));

        LogError("{} returned {}", expr, errBuf);
        //throw std::runtime_error(std::string(expr) + ": " + errBuf);
    }
}
template<typename T>
T* _AV_CHECK_NULL(T* x, const char* msg)
{
    if (x) return x;
    throw std::runtime_error(msg);
}

struct StreamEncoder
{
    AVFormatContext* Muxer = nullptr;
    AVStream* Stream = nullptr;
    AVCodecContext* Encoder = nullptr;

    void OpenMuxer(const fs::path& filename)
    {
        fs::create_directories(filename.parent_path());
        auto str_u = filename.u8string();
        auto str = (char*)str_u.data();
        AV_CHECK(avformat_alloc_output_context2(&Muxer, NULL, NULL, str));
        AV_CHECK(avio_open(&Muxer->pb, str, AVIO_FLAG_WRITE));
    }

    void WriteHeader()
    {
        AV_CHECK(avformat_write_header(Muxer, NULL));
    }
    void WriteTrailer()
    {
        AV_CHECK(av_write_trailer(Muxer));

        avcodec_free_context(&Encoder);
        avformat_free_context(Muxer);
    }

    void AddStream(AVCodecContext* encoder)
    {
        Encoder = encoder;
        Stream = avformat_new_stream(Muxer, encoder->codec);
        Stream->id = Stream->index;

        if (Muxer->oformat->flags & AVFMT_GLOBALHEADER) {
            encoder->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
        AV_CHECK(avcodec_open2(encoder, NULL, NULL));
        AV_CHECK(avcodec_parameters_from_context(Stream->codecpar, encoder));
    }
    AVStream* AddStream(AVMediaType type, AVCodecID codecId)
    {
        Stream = avformat_new_stream(Muxer, nullptr);
        Stream->id = Stream->index;

        auto par = Stream->codecpar;
        par->codec_type = type;
        par->codec_id = codecId;
        return Stream;
    }

    void EncodeFrame(AVFrame* frame)
    {
        AV_CHECK(avcodec_send_frame(Encoder, frame));
        AVPacket* packet = av_packet_alloc();

        while (true) {
            int ret = avcodec_receive_packet(Encoder, packet);

            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
            AV_CHECK(ret);

            packet->stream_index = Stream->index;
            av_packet_rescale_ts(packet, Encoder->time_base, Stream->time_base);

            WritePacket(packet);
        }
        av_packet_free(&packet);
    }
    void WritePacket(AVPacket* packet)
    {
        AV_CHECK(av_interleaved_write_frame(Muxer, packet));
    }
};

struct CdmWrapper;

HMODULE _thisModule;
fs::path _baseDir;
std::unordered_map<media::DecoderStream*, StreamEncoder> _encoders;

AVCodecID ToLavCodec(media::AudioCodec codec)
{
    switch (codec) {
        case media::AudioCodec::kAAC:    return AV_CODEC_ID_AAC;
        case media::AudioCodec::kMP3:    return AV_CODEC_ID_AC3;
        case media::AudioCodec::kVorbis: return AV_CODEC_ID_VORBIS;
        case media::AudioCodec::kFLAC:   return AV_CODEC_ID_FLAC;
        case media::AudioCodec::kOpus:   return AV_CODEC_ID_OPUS;
        case media::AudioCodec::kEAC3:   return AV_CODEC_ID_EAC3;
        case media::AudioCodec::kAC3:    return AV_CODEC_ID_AC3;
        case media::AudioCodec::kDTS:    return AV_CODEC_ID_DTS;
        default: {
            LogWarn("Unknown audio codec {}", (int)codec);
            return AV_CODEC_ID_NONE;
        }
    }
}
DETOUR_FUNC(void, AudioDecoderStream_OnBufferReady, (
    media::DecoderStream* self,
    media::DemuxerStream::Status status,
    media::DecoderBuffer* buffer
))
{
    if (status == media::DemuxerStream::kOk && buffer) {
        LogTrace("AudioDecoderStream::OnBufferReady(): self={} ts={}", (void*)self, buffer->timestamp);

        auto& enc = _encoders[self];
        if (!enc.Muxer) {
            enc.OpenMuxer(_baseDir / std::format("audio_{}_{}.mkv", (void*)self, buffer->timestamp));
            auto config = self->stream_->audio_decoder_config();

            auto stream = enc.AddStream(AVMEDIA_TYPE_AUDIO, ToLavCodec(config.codec_));
            auto par = stream->codecpar;
            par->sample_rate = config.samples_per_second_;
            par->channels = config.channels_;
            par->initial_padding = config.codec_delay_;
            //TODO: channel_layout
            auto& extradata = config.extra_data_;
            par->extradata = (uint8_t*)av_memdup(extradata.data(), extradata.size());
            par->extradata_size = extradata.size();

            enc.WriteHeader();
        }
        AVPacket packet = {};
        packet.stream_index = 0;
        packet.data = buffer->data();
        packet.size = buffer->size_;
        packet.pts = buffer->timestamp;
        packet.duration = buffer->duration;

        av_packet_rescale_ts(&packet, { 1, 1000000 }, enc.Stream->time_base);
        enc.WritePacket(&packet);
    }
    
    return AudioDecoderStream_OnBufferReady_Orig(self, status, buffer);
}
DETOUR_FUNC(void, VideoDecoderStream_OnBufferReady, (
    media::DecoderStream* self,
    media::DemuxerStream::Status status,
    media::DecoderBuffer* buffer
))
{
    if (buffer) {
        LogTrace("VideoDecoderStream::OnBufferReady(): self={} status={} buf_size={} ts={}",
                 (void*)self, (int)status, buffer->size_ , buffer->timestamp, self->cdm_context_);
    }
    return VideoDecoderStream_OnBufferReady_Orig(self, status, buffer);
}
DETOUR_FUNC(void, VideoDecoderStream_OnDecodeOutputReady, (
    media::DecoderStream* self,
    media::VideoFrame* frame
))
{
    auto& size = frame->layout_.coded_size_;

    LogTrace("VideoDecoderStream::OnDecodeOutputReady(): self={} ts={} coded_size={}x{} fmt={}", 
             (void*)self, frame->timestamp_.value, size.width, size.height, (int)frame->layout_.format_);

    auto& enc = _encoders[self];
    if (!enc.Muxer) {
        enc.OpenMuxer(_baseDir / std::format("video_{}_{}.mkv", (void*)self, frame->timestamp_.value));

        AVCodec* codec = AV_CHECK_NULL(avcodec_find_encoder_by_name("libx264"));
        AVCodecContext* encoder = AV_CHECK_NULL(avcodec_alloc_context3(codec));
        encoder->width = size.width;
        encoder->height = size.height;
        encoder->pix_fmt = AV_PIX_FMT_YUV420P;
        encoder->time_base = { 1, 1000000 }; //microseconds
        encoder->gop_size = 16;
        encoder->me_cmp = FF_CMP_SAD; //disable motion estimation on chroma channels
        av_opt_set(encoder->priv_data, "preset", "ultrafast", 0);
        av_opt_set(encoder->priv_data, "crf", "0", 0);

        enc.AddStream(encoder);
        enc.WriteHeader();
    }

    AVFrame frm = {};
    frm.width = size.width;
    frm.height = size.height;
    frm.format = AV_PIX_FMT_YUV420P;
    frm.pts = frame->timestamp_.value;

    for (int i = 0; i < 3; i++) {
        frm.data[i] = frame->data_[i];
        frm.linesize[i] = frame->layout_.planes_[i].stride;
    }
    enc.EncodeFrame(&frm);

    return VideoDecoderStream_OnDecodeOutputReady_Orig(self, frame);
}

fs::path GetCurrentDir()
{
    WCHAR buf[MAX_PATH];
    DWORD len = GetModuleFileName(_thisModule, buf, MAX_PATH);
    return fs::path(buf, buf + len).parent_path();
}

DWORD WINAPI Init(LPVOID param)
{
    _baseDir = GetCurrentDir();
    InitLogger(true, _baseDir / "log.txt");

    MH_Initialize();

    auto chromeMod = (char*)GetModuleHandle(L"chrome.dll");

    //98.0.4758.102
    //https://chromium-browser-symsrv.commondatastorage.googleapis.com/chrome.dll.pdb/29816F82B6D5CFDE4C4C44205044422E1/chrome.dll.pdb
    CREATE_HOOK(chromeMod + 0x008589A0, AudioDecoderStream_OnBufferReady);
    CREATE_HOOK(chromeMod + 0x00D59D80, VideoDecoderStream_OnBufferReady);

    CREATE_HOOK(chromeMod + 0x00C23B40, VideoDecoderStream_OnDecodeOutputReady);
    
    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

#if _WINDLL

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        _thisModule = hModule;
        CreateThread(NULL, 0, Init, NULL, 0, NULL);
    }
    return TRUE;
}

#else

int main()
{
    return 0;
}
#endif