device_filesystem_list_open_files (Device *device,
                                   DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_mounted || device->priv->device_mount_paths->len == 0)
    {
      throw_error (context, ERROR_FAILED, "Device is not mounted");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.filesystem-lsof-system-internal"
                           : "org.freedesktop.udisks.filesystem-lsof",
                           "FilesystemListOpenFiles",
                           TRUE,
                           device_filesystem_list_open_files_authorized_cb,
                           context,
                           0);

 out:
  return TRUE;
}
