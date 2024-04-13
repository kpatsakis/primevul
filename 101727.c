DownloadItemImpl::~DownloadItemImpl() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  DCHECK(!download_file_.get());
  CHECK(!is_updating_observers_);

  for (auto& observer : observers_)
    observer.OnDownloadDestroyed(this);
  delegate_->AssertStateConsistent(this);
  delegate_->Detach();
}
