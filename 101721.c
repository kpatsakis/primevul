void DeleteDownloadedFileDone(
    base::WeakPtr<DownloadItemImpl> item,
    const base::Callback<void(bool)>& callback,
    bool success) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (success && item.get())
    item->OnDownloadedFileRemoved();
  callback.Run(success);
}
