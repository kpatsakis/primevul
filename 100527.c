void FileSystemOperation::DoCreateDirectory(
    const StatusCallback& callback,
    bool exclusive, bool recursive) {
  FileSystemFileUtilProxy::CreateDirectory(
      &operation_context_,
      src_util_, src_path_, exclusive, recursive,
      base::Bind(&FileSystemOperation::DidFinishFileOperation,
                 base::Owned(this), callback));
}
