device_drive_inhibit_polling (Device *device,
                              char **options,
                              DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (!device->priv->device_is_media_change_detection_inhibitable)
    {
      throw_error (context, ERROR_FAILED, "Media detection cannot be inhibited");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.inhibit-polling",
                           "DriveInhibitPolling",
                           TRUE,
                           device_drive_inhibit_polling_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
