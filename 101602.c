void RendererSchedulerImpl::VirtualTimeResumed() {
  for (const auto& pair : task_runners_) {
    if (pair.first->queue_class() == MainThreadTaskQueue::QueueClass::kTimer) {
      DCHECK(!task_queue_throttler_->IsThrottled(pair.first.get()));
      DCHECK(pair.first->HasActiveFence());
      pair.first->RemoveFence();
    }
  }
}
