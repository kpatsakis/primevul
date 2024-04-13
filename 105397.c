device_drive_ata_smart_initiate_selftest (Device *device,
                                          const char *test,
                                          gchar **options,
                                          DBusGMethodInvocation *context)
{
  if (!device->priv->drive_ata_smart_is_available)
    {
      throw_error (context, ERROR_FAILED, "Device does not support ATA SMART");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.drive-ata-smart-selftest",
                           "DriveAtaSmartInitiateSelftest",
                           TRUE,
                           device_drive_ata_smart_initiate_selftest_authorized_cb,
                           context,
                           2,
                           g_strdup (test),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
