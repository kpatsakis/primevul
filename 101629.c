SessionStore::WriteBatch::~WriteBatch() {
  DCHECK(!batch_) << "Destructed without prior commit";
}
