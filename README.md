# ChromeFrameDumper
Ripping video streams by intercepting chromium media pipeline (bypasses L3 widevine)

# Notes
This is a POC, and will only work with Chrome 98.0.4758.102 launched with `--no-sandbox`.

Quality should be in theory better than screen recording, since video frames are obtained directly from the media pipeline, avoiding any additional losses caused by YUV<->RGB conversion. It also eliminates the problem of player controls showing in the rip. Audio can be effectively "WEB-DLed" (not reencoded) because widevine does not support `SW_SECURE_DECODE` for any audio codec (at the time of writing this).

In any way, this is unusable in the current state: streams will be likely split into multiple parts, parts will be missing some frames at the end (encoder/muxer is not flushed), A/V syncing issues, crashes, possible watermarking, ...
