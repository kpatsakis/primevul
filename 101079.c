void StartupTimeBomb::DeleteStartupWatchdog() {
  DCHECK_EQ(thread_id_, base::PlatformThread::CurrentId());
  if (startup_watchdog_->IsJoinable()) {
    base::ThreadRestrictions::SetIOAllowed(true);
    delete startup_watchdog_;
    startup_watchdog_ = nullptr;
    return;
  }
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, base::Bind(&StartupTimeBomb::DeleteStartupWatchdog,
                            base::Unretained(this)),
      base::TimeDelta::FromSeconds(10));
}
