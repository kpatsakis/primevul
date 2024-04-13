void AppCacheGroup::ScheduleUpdateRestart(int delay_ms) {
  DCHECK(restart_update_task_.IsCancelled());
  restart_update_task_.Reset(
      base::Bind(&AppCacheGroup::RunQueuedUpdates, this));
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, restart_update_task_.callback(),
      base::TimeDelta::FromMilliseconds(delay_ms));
}
