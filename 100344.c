void FFmpegVideoDecodeEngine::DecodeFrame(scoped_refptr<Buffer> buffer) {
  scoped_refptr<VideoFrame> video_frame;

  AVPacket packet;
  av_init_packet(&packet);
  packet.data = const_cast<uint8*>(buffer->GetData());
  packet.size = buffer->GetDataSize();

  PipelineStatistics statistics;
  statistics.video_bytes_decoded = buffer->GetDataSize();

  codec_context_->reordered_opaque = buffer->GetTimestamp().InMicroseconds();

  av_frame_->reordered_opaque = codec_context_->reordered_opaque;

  int frame_decoded = 0;
  int result = avcodec_decode_video2(codec_context_,
                                     av_frame_.get(),
                                     &frame_decoded,
                                     &packet);

  if (result < 0) {
    LOG(ERROR) << "Error decoding a video frame with timestamp: "
               << buffer->GetTimestamp().InMicroseconds() << " us, duration: "
               << buffer->GetDuration().InMicroseconds() << " us, packet size: "
               << buffer->GetDataSize() << " bytes";
    event_handler_->OnError();
    return;
  }

  if (frame_decoded == 0) {
    if (buffer->IsEndOfStream()) {  // We had started flushing.
      event_handler_->ConsumeVideoFrame(video_frame, statistics);
      output_eos_reached_ = true;
    } else {
      ReadInput();
    }
    return;
  }

  if (!av_frame_->data[VideoFrame::kYPlane] ||
      !av_frame_->data[VideoFrame::kUPlane] ||
      !av_frame_->data[VideoFrame::kVPlane]) {
    event_handler_->OnError();
    return;
  }

  DCHECK_LE(av_frame_->repeat_pict, 2);  // Sanity check.
  AVRational doubled_time_base;
  doubled_time_base.num = frame_rate_denominator_;
  doubled_time_base.den = frame_rate_numerator_ * 2;

  base::TimeDelta timestamp =
      base::TimeDelta::FromMicroseconds(av_frame_->reordered_opaque);
  base::TimeDelta duration =
      ConvertFromTimeBase(doubled_time_base, 2 + av_frame_->repeat_pict);

  DCHECK(frame_queue_available_.size());
  video_frame = frame_queue_available_.front();
  frame_queue_available_.pop_front();

  size_t height = codec_context_->height;
  CopyPlane(VideoFrame::kYPlane, video_frame.get(), av_frame_.get(), height);
  CopyPlane(VideoFrame::kUPlane, video_frame.get(), av_frame_.get(), height);
  CopyPlane(VideoFrame::kVPlane, video_frame.get(), av_frame_.get(), height);

  video_frame->SetTimestamp(timestamp);
  video_frame->SetDuration(duration);

  pending_output_buffers_--;
  event_handler_->ConsumeVideoFrame(video_frame, statistics);
}
