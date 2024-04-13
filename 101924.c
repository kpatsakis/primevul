  OpenRequest(scoped_refptr<IndexedDBDatabase> db,
              std::unique_ptr<IndexedDBPendingConnection> pending_connection)
      : ConnectionRequest(db),
        pending_(std::move(pending_connection)),
        weak_factory_(this) {}
