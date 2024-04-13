device_filesystem_unmount (Device *device,
                           char **options,
                           DBusGMethodInvocation *context)
{
  const gchar *action_id;
  uid_t uid_of_mount;

  if (!device->priv->device_is_mounted || device->priv->device_mount_paths->len == 0)
    {
      throw_error (context, ERROR_FAILED, "Device is not mounted");
      goto out;
    }

  /* if device is in /etc/fstab, then we'll run unmount as the calling user */
  action_id = NULL;
  if (!mount_file_has_device (device->priv->device_file, &uid_of_mount, NULL))
    {
      if (!is_device_in_fstab (device, NULL))
        {
          action_id = "org.freedesktop.udisks.filesystem-unmount-others";
        }
    }
  else
    {
      uid_t uid;
      daemon_local_get_uid (device->priv->daemon, &uid, context);
      if (uid_of_mount != uid)
        {
          action_id = "org.freedesktop.udisks.filesystem-unmount-others";
        }
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           action_id,
                           "FilesystemUnmount",
                           TRUE,
                           device_filesystem_unmount_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
