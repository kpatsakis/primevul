std::unique_ptr<BlobDataHandle> BlobStorageContext::AddFutureBlob(
    const std::string& uuid,
    const std::string& content_type,
    const std::string& content_disposition) {
  DCHECK(!registry_.HasEntry(uuid));

  BlobEntry* entry =
      registry_.CreateEntry(uuid, content_type, content_disposition);
  entry->set_size(DataElement::kUnknownSize);
  entry->set_status(BlobStatus::PENDING_CONSTRUCTION);
  entry->set_building_state(base::MakeUnique<BlobEntry::BuildingState>(
      false, TransportAllowedCallback(), 0));
  return CreateHandle(uuid, entry);
}
