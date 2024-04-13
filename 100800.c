void DevToolsWindowBeforeUnloadObserver::BeforeUnloadFired(
    const base::TimeTicks& proceed_time) {
  m_fired = true;
  if (message_loop_runner_.get())
    message_loop_runner_->Quit();
}
