Status IndexedDBDatabase::VersionChangeOperation(
    int64_t version,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1(
      "IndexedDBDatabase::VersionChangeOperation", "txn.id", transaction->id());
  int64_t old_version = metadata_.version;
  DCHECK_GT(version, old_version);

  metadata_coding_->SetDatabaseVersion(
      transaction->BackingStoreTransaction()->transaction(), id(), version,
      &metadata_);

  transaction->ScheduleAbortTask(base::BindOnce(
      &IndexedDBDatabase::VersionChangeAbortOperation, this, old_version));

  active_request_->UpgradeTransactionStarted(old_version);
  return Status::OK();
}
