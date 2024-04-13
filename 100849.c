  void RestartHost() {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    if (restarting_ || shutting_down_)
      return;

    restarting_ = true;
    host_->Shutdown(base::Bind(
        &HostProcess::RestartOnHostShutdown, base::Unretained(this)));
  }
