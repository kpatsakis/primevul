base::SequencedTaskRunner& RenderProcessHostImpl::GetAecDumpFileTaskRunner() {
  if (!audio_debug_recordings_file_task_runner_) {
    audio_debug_recordings_file_task_runner_ =
        base::CreateSequencedTaskRunnerWithTraits(
            {base::MayBlock(), base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN,
             base::TaskPriority::USER_BLOCKING});
  }
  return *audio_debug_recordings_file_task_runner_;
}
