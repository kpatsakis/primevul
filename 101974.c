void FileSystemManagerImpl::BindRequest(
    blink::mojom::FileSystemManagerRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!operation_runner_)
    operation_runner_ = context_->CreateFileSystemOperationRunner();
  bindings_.AddBinding(this, std::move(request));
}
