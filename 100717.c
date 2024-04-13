void RenderThreadImpl::HistogramCustomizer::RenderViewNavigatedToHost(
    const std::string& host, size_t view_count) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableHistogramCustomizer)) {
    return;
  }
  if (view_count == 1)
    SetCommonHost(host);
  else if (host != common_host_)
    SetCommonHost(std::string());
}
