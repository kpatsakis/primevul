void IndexedDBDatabase::Commit(IndexedDBTransaction* transaction) {
  if (transaction) {
    scoped_refptr<IndexedDBFactory> factory = factory_;
    Status result = transaction->Commit();
    if (!result.ok())
      ReportError(result);
  }
}
