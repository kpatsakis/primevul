RenderThreadImpl::RenderThreadImpl(const std::string& channel_name)
    : ChildThread(Options(channel_name, ShouldUseMojoChannel())) {
  Init();
}
