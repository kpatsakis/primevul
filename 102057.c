RenderProcessHostWatcher::RenderProcessHostWatcher(
    RenderProcessHost* render_process_host, WatchType type)
    : render_process_host_(render_process_host),
      type_(type),
      did_exit_normally_(true),
      message_loop_runner_(new MessageLoopRunner) {
  render_process_host_->AddObserver(this);
}
