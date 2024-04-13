BlobStorageContext::BlobStorageContext(
    base::FilePath storage_directory,
    scoped_refptr<base::TaskRunner> file_runner)
    : memory_controller_(std::move(storage_directory), std::move(file_runner)),
      ptr_factory_(this) {}
