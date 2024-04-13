void IndexedDBDatabase::OpenConnection(
    std::unique_ptr<IndexedDBPendingConnection> connection) {
  AppendRequest(std::make_unique<OpenRequest>(this, std::move(connection)));
}
