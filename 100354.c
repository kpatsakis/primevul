void ExtensionService::OnExtensionInstalled(
    const Extension* extension, bool from_webstore, int page_index) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  scoped_refptr<const Extension> scoped_extension(extension);
  const std::string& id = extension->id();
  bool initial_enable =
      !extension_prefs_->IsExtensionDisabled(id) ||
      !Extension::UserMayDisable(extension->location());
  PendingExtensionInfo pending_extension_info;
  if (pending_extension_manager()->GetById(id, &pending_extension_info)) {
    pending_extension_manager()->Remove(id);

    if (!pending_extension_info.ShouldAllowInstall(*extension)) {
      LOG(WARNING)
          << "ShouldAllowInstall() returned false for "
          << id << " of type " << extension->GetType()
          << " and update URL " << extension->update_url().spec()
          << "; not installing";

      NotificationService::current()->Notify(
          chrome::NOTIFICATION_EXTENSION_INSTALL_NOT_ALLOWED,
          Source<Profile>(profile_),
          Details<const Extension>(extension));

      if (!BrowserThread::PostTask(
              BrowserThread::FILE, FROM_HERE,
              NewRunnableFunction(&extension_file_util::DeleteFile,
                                  extension->path(), true)))
        NOTREACHED();
      return;
    }
  } else {
    if (IsExternalExtensionUninstalled(id)) {
      initial_enable = true;
    }
  }

  if (!GetExtensionByIdInternal(extension->id(), true, true, false)) {
    UMA_HISTOGRAM_ENUMERATION("Extensions.InstallType",
                              extension->GetType(), 100);
    RecordPermissionMessagesHistogram(
        extension, "Extensions.Permissions_Install");
  }

  ShownSectionsHandler::OnExtensionInstalled(profile_->GetPrefs(), extension);
  extension_prefs_->OnExtensionInstalled(
      extension,
      initial_enable ? Extension::ENABLED : Extension::DISABLED,
      from_webstore,
      page_index);

  if (Extension::ShouldAlwaysAllowFileAccess(extension->location()) &&
      !extension_prefs_->HasAllowFileAccessSetting(id)) {
    extension_prefs_->SetAllowFileAccess(id, true);
  }

  NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_INSTALLED,
      Source<Profile>(profile_),
      Details<const Extension>(extension));

  AddExtension(scoped_extension);
}
