void FileSystemOperationRunner::FinishOperation(OperationID id) {
  scoped_refptr<FileSystemContext> context(file_system_context_);

  auto found = write_target_urls_.find(id);
  if (found != write_target_urls_.end()) {
    const FileSystemURLSet& urls = found->second;
    for (const FileSystemURL& url : urls) {
      if (file_system_context_->GetUpdateObservers(url.type())) {
        file_system_context_->GetUpdateObservers(url.type())
            ->Notify(&FileUpdateObserver::OnEndUpdate, url);
      }
    }
    write_target_urls_.erase(found);
  }

  operations_.erase(id);
  finished_operations_.erase(id);

  auto found_cancel = stray_cancel_callbacks_.find(id);
  if (found_cancel != stray_cancel_callbacks_.end()) {
    std::move(found_cancel->second)
        .Run(base::File::FILE_ERROR_INVALID_OPERATION);
    stray_cancel_callbacks_.erase(found_cancel);
  }
}
