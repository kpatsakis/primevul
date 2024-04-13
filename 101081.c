bool ThreadWatcherList::IsRegistered(const BrowserThread::ID thread_id) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  return nullptr != ThreadWatcherList::Find(thread_id);
}
