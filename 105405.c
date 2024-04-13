device_drive_eject (Device *device,
                    char **options,
                    DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (!device->priv->device_is_media_available)
    {
      throw_error (context, ERROR_FAILED, "No media in drive");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.drive-eject",
                           "DriveEject",
                           TRUE,
                           device_drive_eject_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
