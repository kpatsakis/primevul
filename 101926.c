bool IndexedDBDatabase::ValidateObjectStoreId(int64_t object_store_id) const {
  if (!base::ContainsKey(metadata_.object_stores, object_store_id)) {
    DLOG(ERROR) << "Invalid object_store_id";
    return false;
  }
  return true;
}
