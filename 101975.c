void FileSystemManagerImpl::DidGetMetadata(ReadMetadataCallback callback,
                                           base::File::Error result,
                                           const base::File::Info& info) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::move(callback).Run(info, result);
}
