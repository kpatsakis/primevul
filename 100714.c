RenderThreadImpl::GetMediaThreadMessageLoopProxy() {
  DCHECK(message_loop() == base::MessageLoop::current());
  if (!media_thread_) {
    media_thread_.reset(new base::Thread("Media"));
    media_thread_->Start();

#if defined(OS_ANDROID)
    renderer_demuxer_ = new RendererDemuxerAndroid();
    AddFilter(renderer_demuxer_.get());
#endif
  }
  return media_thread_->message_loop_proxy();
}
