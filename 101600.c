scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::TimerTaskQueue() {
  helper_.CheckOnValidThread();
  return default_timer_task_queue_;
}
