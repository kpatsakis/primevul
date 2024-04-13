void ExtensionService::AddExtension(const Extension* extension) {
  scoped_refptr<const Extension> scoped_extension(extension);

  if (!extensions_enabled() &&
      !extension->is_theme() &&
      extension->location() != Extension::COMPONENT &&
      !Extension::IsExternalLocation(extension->location()))
    return;

  SetBeingUpgraded(extension, false);

  unloaded_extension_paths_.erase(extension->id());

  UntrackTerminatedExtension(extension->id());

  if (disabled_extension_paths_.erase(extension->id()) > 0)
    EnableExtension(extension->id());

  InitializePermissions(extension);

  bool disabled = extension_prefs_->IsExtensionDisabled(extension->id());
  if (disabled) {
    disabled_extensions_.push_back(scoped_extension);
    NotificationService::current()->Notify(
        chrome::NOTIFICATION_EXTENSION_UPDATE_DISABLED,
        Source<Profile>(profile_),
        Details<const Extension>(extension));
    SyncExtensionChangeIfNeeded(*extension);
    return;
  }

  extensions_.push_back(scoped_extension);
  SyncExtensionChangeIfNeeded(*extension);
  NotifyExtensionLoaded(extension);
}
