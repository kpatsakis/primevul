device_drive_set_spindown_timeout (Device *device,
                                   int timeout_seconds,
                                   char **options,
                                   DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (!device->priv->drive_can_spindown)
    {
      throw_error (context, ERROR_FAILED, "Cannot spindown device");
      goto out;
    }

  if (timeout_seconds < 1)
    {
      throw_error (context, ERROR_FAILED, "Timeout seconds must be at least 1");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.drive-set-spindown",
                           "DriveSetSpindownTimeout",
                           TRUE,
                           device_drive_set_spindown_timeout_authorized_cb,
                           context,
                           2,
                           GINT_TO_POINTER (timeout_seconds),
                           NULL,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
