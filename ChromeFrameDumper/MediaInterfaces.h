#pragma once
#include <cstdint>
#include <exception>

//libcxx interop
namespace cxx {
    template<class T>
    struct vector {
        T* begin_;
        T* end_;
        void* end_cap_;

        T& operator[](size_t index) const { return begin_[index]; }

        T* data() { return begin_; }
        size_t size() { return end_ - begin_; }
    };
};

//98.0.4758.82
namespace base {
    struct TimeDelta {
        int64_t value;
    };
}

namespace gfx {
    struct Size {
        int width;
        int height;
    };
    struct Rect {
        int x, y, width, height;
    };
}
//https://source.chromium.org/chromium/chromium/src/+/main:media/base/decoder_buffer.h
namespace media {
    enum class EncryptionScheme {
        kUnencrypted = 0,
        kCenc,  // 'cenc' subsample encryption using AES-CTR mode.
        kCbcs,  // 'cbcs' pattern encryption using AES-CBC mode.
        kMaxValue = kCbcs
    };

    enum class AudioCodec {
        // These values are histogrammed over time; do not change their ordinal
        // values.  When deleting a codec replace it with a dummy value; when adding a
        // codec, do so at the bottom before kMaxValue, and update the value of
        // kMaxValue to equal the new codec.
        kUnknown = 0,
        kAAC = 1,
        kMP3 = 2,
        kPCM = 3,
        kVorbis = 4,
        kFLAC = 5,
        kAMR_NB = 6,
        kAMR_WB = 7,
        kPCM_MULAW = 8,
        kGSM_MS = 9,
        kPCM_S16BE = 10,
        kPCM_S24BE = 11,
        kOpus = 12,
        kEAC3 = 13,
        kPCM_ALAW = 14,
        kALAC = 15,
        kAC3 = 16,
        kMpegHAudio = 17,
        kDTS = 18,
        kDTSXP2 = 19,
        // DO NOT ADD RANDOM AUDIO CODECS!
        //
        // The only acceptable time to add a new codec is if there is production code
        // that uses said codec in the same CL.

        // Must always be equal to the largest entry ever logged.
        kMaxValue = kDTSXP2,
    };

    enum class AudioCodecProfile {
        // These values are histogrammed over time; do not change their ordinal
        // values.  When deleting a profile replace it with a dummy value; when adding
        // a profile, do so at the bottom before kMaxValue, and update the value of
        // kMaxValue to equal the new codec.
        kUnknown = 0,
        kXHE_AAC = 1,
        kMaxValue = kXHE_AAC,
    };

    enum SampleFormat {
        // These values are histogrammed over time; do not change their ordinal
        // values.  When deleting a sample format replace it with a dummy value; when
        // adding a sample format, do so at the bottom before kSampleFormatMax, and
        // update the value of kSampleFormatMax.
        kUnknownSampleFormat = 0,
        kSampleFormatU8,          // Unsigned 8-bit w/ bias of 128.
        kSampleFormatS16,         // Signed 16-bit.
        kSampleFormatS32,         // Signed 32-bit.
        kSampleFormatF32,         // Float 32-bit.
        kSampleFormatPlanarS16,   // Signed 16-bit planar.
        kSampleFormatPlanarF32,   // Float 32-bit planar.
        kSampleFormatPlanarS32,   // Signed 32-bit planar.
        kSampleFormatS24,         // Signed 24-bit.
        kSampleFormatAc3,         // Compressed AC3 bitstream.
        kSampleFormatEac3,        // Compressed E-AC3 bitstream.
        kSampleFormatMpegHAudio,  // Compressed MPEG-H audio bitstream.
        kSampleFormatPlanarU8,    // Unsigned 8-bit w/ bias of 128 planar.
        kSampleFormatDts,         // Compressed DTS audio bitstream.
        kSampleFormatDtsxP2,      // Compressed DTSX audio bitstream.

        // Must always be equal to largest value ever logged.
        kSampleFormatMax = kSampleFormatDtsxP2,
    };

    // Enumerates the various representations of the ordering of audio channels.
    // Logged to UMA, so never reuse a value, always add new/greater ones!
    enum ChannelLayout {
        CHANNEL_LAYOUT_NONE = 0,
        CHANNEL_LAYOUT_UNSUPPORTED = 1,

        // Front C
        CHANNEL_LAYOUT_MONO = 2,

        // Front L, Front R
        CHANNEL_LAYOUT_STEREO = 3,

        // Front L, Front R, Back C
        CHANNEL_LAYOUT_2_1 = 4,

        // Front L, Front R, Front C
        CHANNEL_LAYOUT_SURROUND = 5,

        // Front L, Front R, Front C, Back C
        CHANNEL_LAYOUT_4_0 = 6,

        // Front L, Front R, Side L, Side R
        CHANNEL_LAYOUT_2_2 = 7,

        // Front L, Front R, Back L, Back R
        CHANNEL_LAYOUT_QUAD = 8,

        // Front L, Front R, Front C, Side L, Side R
        CHANNEL_LAYOUT_5_0 = 9,

        // Front L, Front R, Front C, LFE, Side L, Side R
        CHANNEL_LAYOUT_5_1 = 10,

        // Front L, Front R, Front C, Back L, Back R
        CHANNEL_LAYOUT_5_0_BACK = 11,

        // Front L, Front R, Front C, LFE, Back L, Back R
        CHANNEL_LAYOUT_5_1_BACK = 12,

        // Front L, Front R, Front C, Side L, Side R, Back L, Back R
        CHANNEL_LAYOUT_7_0 = 13,

        // Front L, Front R, Front C, LFE, Side L, Side R, Back L, Back R
        CHANNEL_LAYOUT_7_1 = 14,

        // Front L, Front R, Front C, LFE, Side L, Side R, Front LofC, Front RofC
        CHANNEL_LAYOUT_7_1_WIDE = 15,

        // Stereo L, Stereo R
        CHANNEL_LAYOUT_STEREO_DOWNMIX = 16,

        // Stereo L, Stereo R, LFE
        CHANNEL_LAYOUT_2POINT1 = 17,

        // Stereo L, Stereo R, Front C, LFE
        CHANNEL_LAYOUT_3_1 = 18,

        // Stereo L, Stereo R, Front C, Rear C, LFE
        CHANNEL_LAYOUT_4_1 = 19,

        // Stereo L, Stereo R, Front C, Side L, Side R, Back C
        CHANNEL_LAYOUT_6_0 = 20,

        // Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC
        CHANNEL_LAYOUT_6_0_FRONT = 21,

        // Stereo L, Stereo R, Front C, Rear L, Rear R, Rear C
        CHANNEL_LAYOUT_HEXAGONAL = 22,

        // Stereo L, Stereo R, Front C, LFE, Side L, Side R, Rear Center
        CHANNEL_LAYOUT_6_1 = 23,

        // Stereo L, Stereo R, Front C, LFE, Back L, Back R, Rear Center
        CHANNEL_LAYOUT_6_1_BACK = 24,

        // Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC, LFE
        CHANNEL_LAYOUT_6_1_FRONT = 25,

        // Front L, Front R, Front C, Side L, Side R, Front LofC, Front RofC
        CHANNEL_LAYOUT_7_0_FRONT = 26,

        // Front L, Front R, Front C, LFE, Back L, Back R, Front LofC, Front RofC
        CHANNEL_LAYOUT_7_1_WIDE_BACK = 27,

        // Front L, Front R, Front C, Side L, Side R, Rear L, Back R, Back C.
        CHANNEL_LAYOUT_OCTAGONAL = 28,

        // Channels are not explicitly mapped to speakers.
        CHANNEL_LAYOUT_DISCRETE = 29,

        // Deprecated, but keeping the enum value for UMA consistency.
        // Front L, Front R, Front C. Front C contains the keyboard mic audio. This
        // layout is only intended for input for WebRTC. The Front C channel
        // is stripped away in the WebRTC audio input pipeline and never seen outside
        // of that.
        CHANNEL_LAYOUT_STEREO_AND_KEYBOARD_MIC = 30,

        // Front L, Front R, Side L, Side R, LFE
        CHANNEL_LAYOUT_4_1_QUAD_SIDE = 31,

        // Actual channel layout is specified in the bitstream and the actual channel
        // count is unknown at Chromium media pipeline level (useful for audio
        // pass-through mode).
        CHANNEL_LAYOUT_BITSTREAM = 32,

        // Max value, must always equal the largest entry ever logged.
        CHANNEL_LAYOUT_MAX = CHANNEL_LAYOUT_BITSTREAM
    };

    // Note: Do not reorder or reassign these values; other code depends on their
    // ordering to operate correctly. E.g., CoreAudio channel layout computations.
    enum Channels {
        LEFT = 0,
        RIGHT,
        CENTER,
        LFE,
        BACK_LEFT,
        BACK_RIGHT,
        LEFT_OF_CENTER,
        RIGHT_OF_CENTER,
        BACK_CENTER,
        SIDE_LEFT,
        SIDE_RIGHT,
        CHANNELS_MAX = SIDE_RIGHT, // Must always equal the largest value ever logged.
    };

    struct AudioDecoderConfig {
        AudioCodec codec_ = AudioCodec::kUnknown;
        SampleFormat sample_format_ = kUnknownSampleFormat;
        ChannelLayout channel_layout_ = CHANNEL_LAYOUT_UNSUPPORTED;
        int samples_per_second_ = 0;
        cxx::vector<uint8_t> extra_data_;
        EncryptionScheme encryption_scheme_ = EncryptionScheme::kUnencrypted;

        // The duration of data that the decoder must decode before the decoded data
        // is valid.
        base::TimeDelta seek_preroll_;

        // The number of frames the decoder should discard before returning decoded
        // data. Can include both decoder delay and padding added during encoding.
        int codec_delay_ = 0;

        // Optional parameters that can be set later:

        AudioCodecProfile profile_ = AudioCodecProfile::kUnknown;

        // Layout of the output hardware. Optionally set. See setter comments.
        ChannelLayout target_output_channel_layout_ = CHANNEL_LAYOUT_NONE;

        // This is a hack for backward compatibility. For AAC, to preserve existing
        // behavior, we set `aac_extra_data_` on all platforms but only set
        // `extra_data` on Android.
        // TODO(crbug.com/1250841): Remove this after we land a long term fix.
        cxx::vector<uint8_t> aac_extra_data_;

        // Indicates if a decoder should implicitly discard decoder delay without it
        // being explicitly marked in discard padding.
        bool should_discard_decoder_delay_ = true;

        // Derived values from mandatory and optional parameters above:

        int bytes_per_channel_ = 0;
        int bytes_per_frame_ = 0;

        // Count of channels. By default derived from `channel_layout_`, but can also
        // be manually set in `SetChannelsForDiscrete()`;
        int channels_ = 0;
    };
    struct DecryptingDemuxerStream
    {
        char pad[72];
        AudioDecoderConfig audio_config_;
    };
    struct ReadOnlyUnalignedMapping
    {
        void* _vtable;
        uint8_t* memory_;
        size_t mapping_size_;
        size_t mapped_size_;
        uint8_t guid_[16];

        size_t size_;
        size_t misalignment_;
    };
    struct DecoderBuffer
    {
        char pad[16];
        uint8_t* data_;

        int64_t timestamp; //microseconds
        int64_t duration;
        int64_t discard_padding_1;
        int64_t discard_padding_2;

        size_t size_;

        // Side data. Used for alpha channel in VPx, and for text cues.
        size_t side_data_size_;
        uint8_t* side_data_;

        // Encoded data, if it is stored in a shared memory mapping.
        ReadOnlyUnalignedMapping* shared_mem_mapping_;
        // Encoded data, if it is stored in SHM.
        void* shm_;
        // Encryption parameters for the encoded data.
        void* decrypt_config_;
        // Whether the frame was marked as a keyframe in the container.
        bool is_key_frame_;

        bool end_of_stream() const { return !shared_mem_mapping_ && !shm_ && !data_; }
        uint8_t* data() const
        {
            if (shared_mem_mapping_ && shared_mem_mapping_->memory_ != nullptr)
                return shared_mem_mapping_->memory_ + shared_mem_mapping_->misalignment_;
            if (shm_)
                throw std::exception(); //shm_ is deprecated, no point in implementing it
            return data_;
        }
    };
    enum class DecryptorStatus {
        kSuccess,       // Decryption successfully completed. Decrypted buffer ready.
        kNoKey,         // No key is available to decrypt.
        kNeedMoreData,  // Decoder needs more data to produce a frame.
        kError,         // Key is available but an error occurred during decryption.
        kStatusMax = kError
    };

    // GENERATED_JAVA_ENUM_PACKAGE: org.chromium.media
    enum class VideoCodec {
        // These values are histogrammed over time; do not change their ordinal
        // values.  When deleting a codec replace it with a dummy value; when adding a
        // codec, do so at the bottom (and update kMaxValue).
        kUnknown = 0,
        kH264,
        kVC1,
        kMPEG2,
        kMPEG4,
        kTheora,
        kVP8,
        kVP9,
        kHEVC,
        kDolbyVision,
        kAV1,
        // DO NOT ADD RANDOM VIDEO CODECS!
        //
        // The only acceptable time to add a new codec is if there is production code
        // that uses said codec in the same CL.

        kMaxValue = kAV1,  // Must equal the last "real" codec above.
    };
    // Video codec profiles. Keep in sync with mojo::VideoCodecProfile (see
    // media/mojo/mojom/media_types.mojom), gpu::VideoCodecProfile (see
    // gpu/config/gpu_info.h), and PP_VideoDecoder_Profile (translation is performed
    // in content/renderer/pepper/ppb_video_decoder_impl.cc).
    // NOTE: These values are histogrammed over time in UMA so the values must never
    // ever change (add new values to tools/metrics/histograms/histograms.xml)
    // GENERATED_JAVA_ENUM_PACKAGE: org.chromium.media
    enum VideoCodecProfile {
        // Keep the values in this enum unique, as they imply format (h.264 vs. VP8,
        // for example), and keep the values for a particular format grouped
        // together for clarity.
        VIDEO_CODEC_PROFILE_UNKNOWN = -1,
        VIDEO_CODEC_PROFILE_MIN = VIDEO_CODEC_PROFILE_UNKNOWN,
        H264PROFILE_MIN = 0,
        H264PROFILE_BASELINE = H264PROFILE_MIN,
        H264PROFILE_MAIN = 1,
        H264PROFILE_EXTENDED = 2,
        H264PROFILE_HIGH = 3,
        H264PROFILE_HIGH10PROFILE = 4,
        H264PROFILE_HIGH422PROFILE = 5,
        H264PROFILE_HIGH444PREDICTIVEPROFILE = 6,
        H264PROFILE_SCALABLEBASELINE = 7,
        H264PROFILE_SCALABLEHIGH = 8,
        H264PROFILE_STEREOHIGH = 9,
        H264PROFILE_MULTIVIEWHIGH = 10,
        H264PROFILE_MAX = H264PROFILE_MULTIVIEWHIGH,
        VP8PROFILE_MIN = 11,
        VP8PROFILE_ANY = VP8PROFILE_MIN,
        VP8PROFILE_MAX = VP8PROFILE_ANY,
        VP9PROFILE_MIN = 12,
        VP9PROFILE_PROFILE0 = VP9PROFILE_MIN,
        VP9PROFILE_PROFILE1 = 13,
        VP9PROFILE_PROFILE2 = 14,
        VP9PROFILE_PROFILE3 = 15,
        VP9PROFILE_MAX = VP9PROFILE_PROFILE3,
        HEVCPROFILE_MIN = 16,
        HEVCPROFILE_MAIN = HEVCPROFILE_MIN,
        HEVCPROFILE_MAIN10 = 17,
        HEVCPROFILE_MAIN_STILL_PICTURE = 18,
        HEVCPROFILE_MAX = HEVCPROFILE_MAIN_STILL_PICTURE,
        DOLBYVISION_PROFILE0 = 19,
        DOLBYVISION_PROFILE4 = 20,
        DOLBYVISION_PROFILE5 = 21,
        DOLBYVISION_PROFILE7 = 22,
        THEORAPROFILE_MIN = 23,
        THEORAPROFILE_ANY = THEORAPROFILE_MIN,
        THEORAPROFILE_MAX = THEORAPROFILE_ANY,
        AV1PROFILE_MIN = 24,
        AV1PROFILE_PROFILE_MAIN = AV1PROFILE_MIN,
        AV1PROFILE_PROFILE_HIGH = 25,
        AV1PROFILE_PROFILE_PRO = 26,
        AV1PROFILE_MAX = AV1PROFILE_PROFILE_PRO,
        DOLBYVISION_PROFILE8 = 27,
        DOLBYVISION_PROFILE9 = 28,
        VIDEO_CODEC_PROFILE_MAX = DOLBYVISION_PROFILE9,
    };
    
    struct VideoDecoderConfig {
        VideoCodec codec_ = VideoCodec::kUnknown;
        VideoCodecProfile profile_ = VIDEO_CODEC_PROFILE_UNKNOWN;

        char pad1[72 - 12];

        cxx::vector<uint8_t> extra_data_;

        EncryptionScheme encryption_scheme_ = EncryptionScheme::kUnencrypted;

        /*
  [0] codec_: R#75159
  [4] profile_: R#117221
  [8] level_: UInt32
  [12] alpha_mode_: R#1297241
  [16] transformation_: R#833465
  [24] coded_size_: R#96014
  [32] visible_rect_: R#96382
  [48] natural_size_: R#96014
  [56] aspect_ratio_: R#1297151
  [72] extra_data_: R#73140
  [96] encryption_scheme_: R#125513
  [100] color_space_info_: R#833461
  [104] hdr_metadata_: R#74846
  [160] is_rtc_: Boolean8
*/
    };

    // Pixel formats roughly based on FOURCC labels, see:
    // http://www.fourcc.org/rgb.php and http://www.fourcc.org/yuv.php
    // Logged to UMA, so never reuse values. Leave gaps if necessary.
    // Ordered as planar, semi-planar, YUV-packed, and RGB formats.
    // When a VideoFrame is backed by native textures, VideoPixelFormat describes
    // how those textures should be sampled and combined to produce the final
    // pixels.
    enum VideoPixelFormat {
        PIXEL_FORMAT_UNKNOWN = 0,  // Unknown or unspecified format value.
        PIXEL_FORMAT_I420 =
        1,  // 12bpp YUV planar 1x1 Y, 2x2 UV samples, a.k.a. YU12.

        // Note: Chrome does not actually support YVU compositing, so you probably
        // don't actually want to use this. See http://crbug.com/784627.
        PIXEL_FORMAT_YV12 = 2,  // 12bpp YVU planar 1x1 Y, 2x2 VU samples.

        PIXEL_FORMAT_I422 = 3,   // 16bpp YUV planar 1x1 Y, 2x1 UV samples.
        PIXEL_FORMAT_I420A = 4,  // 20bpp YUVA planar 1x1 Y, 2x2 UV, 1x1 A samples.
        PIXEL_FORMAT_I444 = 5,   // 24bpp YUV planar, no subsampling.
        PIXEL_FORMAT_NV12 =
        6,  // 12bpp with Y plane followed by a 2x2 interleaved UV plane.
        PIXEL_FORMAT_NV21 =
        7,  // 12bpp with Y plane followed by a 2x2 interleaved VU plane.
        PIXEL_FORMAT_UYVY =
        8,  // 16bpp interleaved 2x1 U, 1x1 Y, 2x1 V, 1x1 Y samples.
        PIXEL_FORMAT_YUY2 =
        9,  // 16bpp interleaved 1x1 Y, 2x1 U, 1x1 Y, 2x1 V samples.
        PIXEL_FORMAT_ARGB = 10,   // 32bpp BGRA (byte-order), 1 plane.
        PIXEL_FORMAT_XRGB = 11,   // 24bpp BGRX (byte-order), 1 plane.
        PIXEL_FORMAT_RGB24 = 12,  // 24bpp BGR (byte-order), 1 plane.

        /* PIXEL_FORMAT_RGB32 = 13,  Deprecated */
        PIXEL_FORMAT_MJPEG = 14,  // MJPEG compressed.
        /* PIXEL_FORMAT_MT21 = 15,  Deprecated */

        // The P* in the formats below designates the number of bits per pixel
        // component. I.e. P9 is 9-bits per pixel component, P10 is 10-bits per pixel
        // component, etc.
        PIXEL_FORMAT_YUV420P9 = 16,
        PIXEL_FORMAT_YUV420P10 = 17,
        PIXEL_FORMAT_YUV422P9 = 18,
        PIXEL_FORMAT_YUV422P10 = 19,
        PIXEL_FORMAT_YUV444P9 = 20,
        PIXEL_FORMAT_YUV444P10 = 21,
        PIXEL_FORMAT_YUV420P12 = 22,
        PIXEL_FORMAT_YUV422P12 = 23,
        PIXEL_FORMAT_YUV444P12 = 24,

        /* PIXEL_FORMAT_Y8 = 25, Deprecated */
        PIXEL_FORMAT_Y16 = 26,  // single 16bpp plane.

        PIXEL_FORMAT_ABGR = 27,  // 32bpp RGBA (byte-order), 1 plane.
        PIXEL_FORMAT_XBGR = 28,  // 24bpp RGBX (byte-order), 1 plane.

        PIXEL_FORMAT_P016LE = 29,  // 24bpp NV12, 16 bits per channel

        PIXEL_FORMAT_XR30 =
        30,  // 32bpp BGRX, 10 bits per channel, 2 bits ignored, 1 plane
        PIXEL_FORMAT_XB30 =
        31,  // 32bpp RGBX, 10 bits per channel, 2 bits ignored, 1 plane

        PIXEL_FORMAT_BGRA = 32,  // 32bpp ARGB (byte-order), 1 plane.

        PIXEL_FORMAT_RGBAF16 = 33,  // Half float RGBA, 1 plane.

        // Please update UMA histogram enumeration when adding new formats here.
        PIXEL_FORMAT_MAX =
        PIXEL_FORMAT_RGBAF16,  // Must always be equal to largest entry logged.
    };
    // Encapsulates a color plane's memory layout: (stride, offset, size)
    // stride: in bytes of a plane. Note that stride can be negative if the image
    //         layout is bottom-up.
    // offset: in bytes of a plane, which stands for the offset of a start point of
    //         a color plane from a buffer FD.
    // size:   in bytes of a plane. This |size| bytes data must contain all the data
    //         a decoder will access (e.g. visible area and padding).
    struct ColorPlaneLayout {
        int32_t stride = 0;
        size_t offset = 0;
        size_t size = 0;
    };
    // A class to describes how physical buffer is allocated for video frame.
    // In stores format, coded size of the frame and size of physical buffers
    // which can be used to allocate buffer(s) hardware expected.
    // It also stores stride (bytes per line) and offset per color plane as Plane.
    // stride is to calculate each color plane's size (note that a buffer may
    // contains multiple color planes.)
    // offset is to describe a start point of each plane from buffer's dmabuf fd.
    // Note that it is copyable.
    struct VideoFrameLayout {
        // Default alignment for buffers.
        // Note: This value is dependent on what's used by ffmpeg, do not change
        // without inspecting av_frame_get_buffer() first.
        static constexpr size_t kBufferAddressAlignment = 32;

        VideoPixelFormat format_;

        // Width and height of the video frame in pixels. This must include pixel
        // data for the whole image; i.e. for YUV formats with subsampled chroma
        // planes, in the case that the visible portion of the image does not line up
        // on a sample boundary, |coded_size_| must be rounded up appropriately and
        // the pixel data provided for the odd pixels.
        gfx::Size coded_size_;

        // Layout property for each color planes, e.g. stride and buffer offset.
        cxx::vector<ColorPlaneLayout> planes_;

        // Set to true when a format uses multiple backing buffers to store its
        // planes. Used by code for V4L2 API at the moment.
        bool is_multi_planar_;

        // Memory address alignment of the buffers. This is only relevant when
        // allocating physical memory for the buffer, so it doesn't need to be
        // serialized when frames are passed through Mojo.
        size_t buffer_addr_align_;

        // Modifier of buffers. The modifier is retrieved from GBM library. This
        // can be a different value from kNoModifier only if the VideoFrame is created
        // by using NativePixmap.
        uint64_t modifier_;
    };

    struct VideoFrame {
        enum {
            kFrameSizeAlignment = 16,
            kFrameSizePadding = 16,

            kFrameAddressAlignment = VideoFrameLayout::kBufferAddressAlignment
        };

        enum {
            kMaxPlanes = 4,

            kYPlane = 0,
            kARGBPlane = kYPlane,
            kUPlane = 1,
            kUVPlane = kUPlane,
            kVPlane = 2,
            kAPlane = 3,
        };

        // Defines the pixel storage type. Differentiates between directly accessible
        // |data_| and pixels that are only indirectly accessible and not via mappable
        // memory.
        // Note that VideoFrames of any StorageType can also have Texture backing,
        // with "classical" GPU Driver-only textures identified as STORAGE_OPAQUE.
        enum StorageType {
            STORAGE_UNKNOWN = 0,
            STORAGE_OPAQUE = 1,  // We don't know how VideoFrame's pixels are stored.
            STORAGE_UNOWNED_MEMORY = 2,  // External, non owned data pointers.
            STORAGE_OWNED_MEMORY = 3,  // VideoFrame has allocated its own data buffer.
            STORAGE_SHMEM = 4,         // Backed by unsafe (writable) shared memory.
            // TODO(mcasas): Consider turning this type into STORAGE_NATIVE
            // based on the idea of using this same enum value for both DMA
            // buffers on Linux and CVPixelBuffers on Mac (which currently use
            // STORAGE_UNOWNED_MEMORY) and handle it appropriately in all cases.
            STORAGE_DMABUFS = 5,  // Each plane is stored into a DmaBuf.
            // Backed by a mojo shared buffer. This should only be used by the
            // MojoSharedBufferVideoFrame subclass.
            STORAGE_MOJO_SHARED_BUFFER = 6,
            STORAGE_GPU_MEMORY_BUFFER = 7,
            STORAGE_LAST = STORAGE_GPU_MEMORY_BUFFER,
        };

        char pad1[16];

        // VideFrameLayout (includes format, coded_size, and strides).
        const VideoFrameLayout layout_;

        // Set by WrapVideoFrame to soft-apply a new set of format, visible rectangle,
        // and natural size on |wrapped_frame_|
        VideoFrame* wrapped_frame_;

        // Storage type for the different planes.
        StorageType storage_type_;  // TODO(mcasas): make const

        // Width, height, and offsets of the visible portion of the video frame. Must
        // be a subrect of |coded_size_|. Can be odd with respect to the sample
        // boundaries, e.g. for formats with subsampled chroma.
        const gfx::Rect visible_rect_;

        // Width and height of the visible portion of the video frame
        // (|visible_rect_.size()|) with aspect ratio taken into account.
        const gfx::Size natural_size_;

        // Array of data pointers to each plane.
        // TODO(mcasas): we don't know on ctor if we own |data_| or not. Change
        // to std::unique_ptr<uint8_t, AlignedFreeDeleter> after refactoring
        // VideoFrame.
        uint8_t* data_[kMaxPlanes];

        char pad2[480 - 152];

        base::TimeDelta timestamp_;


        /*  [16] layout_: R#834995
        [80] wrapped_frame_: R#74658
        [88] storage_type_: R#1297478
        [92] visible_rect_: R#96383
        [108] natural_size_: R#96015
        [120] data_: R#792230
        [152] mailbox_holders_: R#874484
        [344] mailbox_holders_and_gmb_release_cb_: R#874383
        [352] shm_region_: R#1297479
        [360] owned_shm_region_: R#674501
        [400] owned_shm_mapping_: R#876907
        [448] gpu_memory_buffer_: R#874207
        [456] done_callbacks_: R#403463
        [480] timestamp_: R#71202
        [488] release_sync_token_lock_: R#416841
        [496] release_sync_token_: R#836073
        [520] metadata_: R#834803
        [1064] unique_id_: R#4098
        [1068] color_space_: R#674668
        [1136] hdr_metadata_: R#74846
        [1192] ycbcr_info_: R#834797
        [1248] private_data_: R#1297480*/
    };

    class DemuxerStream {
    public:
        enum Type {
            UNKNOWN,
            AUDIO,
            VIDEO,
            TEXT,
            TYPE_MAX = TEXT,
        };

        enum Liveness {
            LIVENESS_UNKNOWN,
            LIVENESS_RECORDED,
            LIVENESS_LIVE,
            LIVENESS_MAX = LIVENESS_LIVE,
        };

        // Status returned in the Read() callback.
        //  kOk : Indicates the second parameter is Non-NULL and contains media data
        //        or the end of the stream.
        //  kAborted : Indicates an aborted Read(). This can happen if the
        //             DemuxerStream gets flushed and doesn't have any more data to
        //             return. The second parameter MUST be NULL when this status is
        //             returned.
        //  kConfigChange : Indicates that the AudioDecoderConfig or
        //                  VideoDecoderConfig for the stream has changed.
        //                  The DemuxerStream expects an audio_decoder_config() or
        //                  video_decoder_config() call before Read() will start
        //                  returning DecoderBuffers again. The decoder will need this
        //                  new configuration to properly decode the buffers read
        //                  from this point forward. The second parameter MUST be NULL
        //                  when this status is returned.
        //                  This will only be returned if SupportsConfigChanges()
        //                  returns 'true' for this DemuxerStream.
        // kError : Unexpected fatal error happened. Playback should fail.
        enum Status {
            kOk,
            kAborted,
            kConfigChanged,
            kError,
            kStatusMax = kError,
        };

        // Request a buffer to returned via the provided callback.
        //
        // The first parameter indicates the status of the read.
        // The second parameter is non-NULL and contains media data
        // or the end of the stream if the first parameter is kOk. NULL otherwise.
        typedef /*base::OnceCallback<void(Status, scoped_refptr<DecoderBuffer>)>*/ void* ReadCB;
        virtual void Read(ReadCB read_cb) = 0;

        // Returns the audio/video decoder configuration. It is an error to call the
        // audio method on a video stream and vice versa. After |kConfigChanged| is
        // returned in a Read(), the caller should call this method again to retrieve
        // the new config.
        virtual AudioDecoderConfig audio_decoder_config() = 0;
        virtual VideoDecoderConfig video_decoder_config() = 0;

        // Returns the type of stream.
        virtual Type type() const = 0;

        // Returns liveness of the streams provided, i.e. whether recorded or live.
        virtual Liveness liveness() const = 0;

        virtual void EnableBitstreamConverter() = 0;

        // Whether or not this DemuxerStream allows midstream configuration changes.
        //
        // A DemuxerStream that returns 'true' to this may return the 'kConfigChange'
        // status from a Read() call. In this case the client is expected to be
        // capable of taking appropriate action to handle config changes. Otherwise
        // audio_decoder_config() and video_decoder_config()'s return values are
        // guaranteed to remain constant, and the client may make optimizations based
        // on this.
        virtual bool SupportsConfigChanges() = 0;

    protected:
        // Only allow concrete implementations to get deleted.
        virtual ~DemuxerStream() = 0;
    };
    
    class DecoderStream
    {
        char pad1[80];

    public:
        DemuxerStream* stream_;
        void* cdm_context_;

        /*
        
  [8] traits_: R#1358715
  [16] task_runner_: R#71265
  [24] media_log_: R#71947
  [32] state_: R#1359096
  [40] statistics_cb_: R#1358809
  [48] init_cb_: R#141239
  [56] waiting_cb_: R#72096
  [64] read_cb_: R#1359097
  [72] reset_cb_: R#72833
  [80] stream_: R#72040
  [88] cdm_context_: R#72095
  [96] decoder_: R#74002
  [104] decoder_produced_a_frame_: Boolean8
  [112] decrypting_demuxer_stream_: R#72099
  [120] decoder_selector_: R#74495
  [456] config_change_observer_cb_: R#1359098
  [464] decoder_change_observer_cb_: R#1358893
  [472] decoding_eos_: Boolean8
  [480] prepare_cb_: R#1358851
  [488] preparing_output_: Boolean8
  [496] unprepared_outputs_: R#1358935
  [528] ready_outputs_: R#1358935
  [560] pending_decode_requests_: Int32
  [568] duration_tracker_: R#1273340
  [624] pending_buffers_: R#1272673
  [656] fallback_buffers_: R#1272673
  [688] received_config_change_during_reinit_: Boolean8
  [689] pending_demuxer_read_: Boolean8
  [696] skip_prepare_until_timestamp_: R#71202
  [704] encryption_type_reported_: Boolean8
  [712] weak_factory_: R#1359073
  [728] fallback_weak_factory_: R#1359073
  [744] prepare_weak_factory_: R#1359073
*/
    };
}