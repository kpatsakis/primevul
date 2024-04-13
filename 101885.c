std::unique_ptr<BlobDataHandle> BlobStorageContext::CreateHandle(
    const std::string& uuid,
    BlobEntry* entry) {
  return base::WrapUnique(new BlobDataHandle(
      uuid, entry->content_type_, entry->content_disposition_, entry->size_,
      this, base::ThreadTaskRunnerHandle::Get().get()));
}
