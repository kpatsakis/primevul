void ThreadWatcher::DeActivateThreadWatching() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  active_ = false;
  ping_count_ = 0;
  weak_ptr_factory_.InvalidateWeakPtrs();
}
