device_drive_poll_media (Device *device,
                         DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.inhibit-polling",
                           "DrivePollMedia",
                           TRUE,
                           device_drive_poll_media_authorized_cb,
                           context,
                           0);
 out:
  return TRUE;
}
