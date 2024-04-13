void DownloadItemImpl::AddObserver(Observer* observer) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  observers_.AddObserver(observer);
}
