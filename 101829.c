void BrowserChildProcessHostImpl::TerminateOnBadMessageReceived(
    const std::string& error) {
  HistogramBadMessageTerminated(static_cast<ProcessType>(data_.process_type));
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableKillAfterBadIPC)) {
    return;
  }
  LOG(ERROR) << "Terminating child process for bad IPC message: " << error;
  base::debug::DumpWithoutCrashing();

  child_process_->GetProcess().Terminate(RESULT_CODE_KILLED_BAD_MESSAGE, false);
}
