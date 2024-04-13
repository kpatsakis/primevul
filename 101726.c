void DownloadItemImpl::ShowDownloadInShell() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  delegate_->ShowDownloadInShell(this);
}
