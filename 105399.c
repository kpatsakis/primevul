device_drive_ata_smart_refresh_data (Device *device,
                                     char **options,
                                     DBusGMethodInvocation *context)
{
  const gchar *action_id;

  action_id = NULL;
  if (context != NULL)
    {
      action_id = "org.freedesktop.udisks.drive-ata-smart-refresh";
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           action_id,
                           "DriveAtaSmartRefreshData",
                           TRUE,
                           device_drive_ata_smart_refresh_data_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
