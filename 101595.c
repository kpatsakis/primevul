void RendererSchedulerImpl::RemovePendingNavigation(NavigatingFrameType type) {
  helper_.CheckOnValidThread();
  DCHECK_GT(main_thread_only().navigation_task_expected_count, 0);
  if (type == NavigatingFrameType::kMainFrame &&
      main_thread_only().navigation_task_expected_count > 0) {
    main_thread_only().navigation_task_expected_count--;
    UpdatePolicy();
  }
}
