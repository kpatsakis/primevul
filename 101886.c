void BlobStorageContext::NotifyTransportCompleteInternal(BlobEntry* entry) {
  DCHECK(entry);
  for (ShareableBlobDataItem* shareable_item :
       entry->building_state_->transport_items) {
    DCHECK(shareable_item->state() == ShareableBlobDataItem::QUOTA_GRANTED);
    shareable_item->set_state(ShareableBlobDataItem::POPULATED_WITH_QUOTA);
  }
  entry->set_status(BlobStatus::PENDING_INTERNALS);
  if (entry->CanFinishBuilding())
    FinishBuilding(entry);
}
