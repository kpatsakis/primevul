void FileSystemOperation::SyncGetPlatformPath(const GURL& path_url,
                                              FilePath* platform_path) {
  DCHECK(SetPendingOperationType(kOperationGetLocalPath));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_READ);
  if (result != base::PLATFORM_FILE_OK) {
    delete this;
    return;
  }

  src_util_->GetLocalFilePath(
      &operation_context_, src_path_, platform_path);

  delete this;
}
