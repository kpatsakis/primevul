void ExtensionService::DisableExtension(const std::string& extension_id) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!IsExtensionEnabled(extension_id))
    return;

  const Extension* extension = GetInstalledExtension(extension_id);
  if (extension && !Extension::UserMayDisable(extension->location()))
    return;

  extension_prefs_->SetExtensionState(extension_id, Extension::DISABLED);

  extension = GetExtensionByIdInternal(extension_id, true, false, true);
  if (!extension)
    return;

  disabled_extensions_.push_back(make_scoped_refptr(extension));
  ExtensionList::iterator iter = std::find(extensions_.begin(),
                                           extensions_.end(),
                                           extension);
  if (iter != extensions_.end()) {
    extensions_.erase(iter);
  } else {
    iter = std::find(terminated_extensions_.begin(),
                     terminated_extensions_.end(),
                     extension);
    terminated_extensions_.erase(iter);
  }

  NotifyExtensionUnloaded(extension, extension_misc::UNLOAD_REASON_DISABLE);

  SyncExtensionChangeIfNeeded(*extension);
}
