void WatchDogThread::CleanUp() {
  base::AutoLock lock(g_watchdog_lock.Get());
  g_watchdog_thread = nullptr;
}
