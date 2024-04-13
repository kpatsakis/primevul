device_filesystem_set_label (Device *device,
                             const char *new_label,
                             DBusGMethodInvocation *context)
{
  const Filesystem *fs_details;
  GError *error;

  error = NULL;

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "filesystem") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a mountable file system");
      goto out;
    }

  fs_details = daemon_local_get_fs_details (device->priv->daemon, device->priv->id_type);
  if (fs_details == NULL)
    {
      throw_error (context, ERROR_BUSY, "Unknown filesystem");
      goto out;
    }

  if (!fs_details->supports_online_label_rename)
    {
      if (device_local_is_busy (device, FALSE, &error))
        {
          dbus_g_method_return_error (context, error);
          g_error_free (error);
          goto out;
        }
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "FilesystemSetLabel",
                           TRUE,
                           device_filesystem_set_label_authorized_cb,
                           context,
                           1,
                           g_strdup (new_label),
                           g_free);

 out:
  return TRUE;
}
