bool ExtensionService::SyncBundle::HasExtensionId(const std::string& id) const {
  return synced_extensions.find(id) != synced_extensions.end();
}
