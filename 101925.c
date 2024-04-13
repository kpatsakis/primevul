void IndexedDBDatabase::RegisterAndScheduleTransaction(
    IndexedDBTransaction* transaction) {
  IDB_TRACE1("IndexedDBDatabase::RegisterAndScheduleTransaction", "txn.id",
             transaction->id());
  std::vector<ScopesLockManager::ScopeLockRequest> lock_requests;
  lock_requests.reserve(1 + transaction->scope().size());
  lock_requests.emplace_back(
      kDatabaseRangeLockLevel, GetDatabaseLockRange(id()),
      transaction->mode() == blink::mojom::IDBTransactionMode::VersionChange
          ? ScopesLockManager::LockType::kExclusive
          : ScopesLockManager::LockType::kShared);
  ScopesLockManager::LockType lock_type =
      transaction->mode() == blink::mojom::IDBTransactionMode::ReadOnly
          ? ScopesLockManager::LockType::kShared
          : ScopesLockManager::LockType::kExclusive;
  for (int64_t object_store : transaction->scope()) {
    lock_requests.emplace_back(kObjectStoreRangeLockLevel,
                               GetObjectStoreLockRange(id(), object_store),
                               lock_type);
  }
  lock_manager_->AcquireLocks(
      std::move(lock_requests),
      base::BindOnce(&IndexedDBTransaction::Start, transaction->AsWeakPtr()));
}
