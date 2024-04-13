  void SigTermHandler(int signal_number) {
    DCHECK(signal_number == SIGTERM);
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());
    LOG(INFO) << "Caught SIGTERM: Shutting down...";
    Shutdown(kSuccessExitCode);
  }
