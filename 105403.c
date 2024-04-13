device_drive_detach (Device *device,
                     char **options,
                     DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (!device->priv->drive_can_detach)
    {
      throw_error (context, ERROR_FAILED, "Device is not detachable");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.drive-detach",
                           "DriveDetach",
                           TRUE,
                           device_drive_detach_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
