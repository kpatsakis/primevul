FFmpegVideoDecoder::~FFmpegVideoDecoder() {
  DCHECK_EQ(kUninitialized, state_);
  DCHECK(!codec_context_);
  DCHECK(!av_frame_);
}
