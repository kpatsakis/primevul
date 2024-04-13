void FileSystemOperation::DirectoryExists(const GURL& path_url,
                                          const StatusCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationDirectoryExists));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_READ);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result);
    delete this;
    return;
  }

  FileSystemFileUtilProxy::GetFileInfo(
      &operation_context_, src_util_, src_path_,
      base::Bind(&FileSystemOperation::DidDirectoryExists,
                 base::Owned(this), callback));
}
