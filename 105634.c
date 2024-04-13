update_info_luks_cleartext (Device *device)
{
  uid_t unlocked_by_uid;
  const gchar *dkd_dm_name;
  const gchar *dkd_dm_target_types;
  gboolean ret;

  ret = FALSE;

  dkd_dm_name = g_udev_device_get_property (device->priv->d, "DM_NAME");
  dkd_dm_target_types = g_udev_device_get_property (device->priv->d, "UDISKS_DM_TARGETS_TYPE");
  if (dkd_dm_name != NULL && g_strcmp0 (dkd_dm_target_types, "crypt") == 0 && device->priv->slaves_objpath->len == 1)
    {

      /* TODO: might be racing with setting is_drive earlier */
      device_set_device_is_drive (device, FALSE);

      if (g_str_has_prefix (dkd_dm_name, "temporary-cryptsetup-"))
        {
          /* ignore temporary devices created by /sbin/cryptsetup */
          goto out;
        }

      device_set_device_is_luks_cleartext (device, TRUE);

      device_set_luks_cleartext_slave (device, ((gchar **) device->priv->slaves_objpath->pdata)[0]);

      if (luks_get_uid_from_dm_name (dkd_dm_name, &unlocked_by_uid))
        {
          device_set_luks_cleartext_unlocked_by_uid (device, unlocked_by_uid);
        }

      /* TODO: export this at some point */
      device_set_dm_name (device, dkd_dm_name);
    }
  else
    {
      device_set_device_is_luks_cleartext (device, FALSE);
      device_set_luks_cleartext_slave (device, NULL);
    }

  ret = TRUE;

 out:
  return ret;
}
