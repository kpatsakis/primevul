void ThreadWatcherObserver::RemoveNotifications() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (!g_thread_watcher_observer_)
    return;
  g_thread_watcher_observer_->registrar_.RemoveAll();
  delete g_thread_watcher_observer_;
}
