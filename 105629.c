update_info_linux_lvm2_lv (Device *device)
{
  const gchar *lv_name;
  const gchar *vg_name;
  const gchar *uuid;
  gchar *lv_uuid;
  gchar *vg_uuid;
  gboolean is_lv;

  is_lv = FALSE;
  lv_uuid = NULL;
  vg_uuid = NULL;

  lv_name = g_udev_device_get_property (device->priv->d, "DM_LV_NAME");
  vg_name = g_udev_device_get_property (device->priv->d, "DM_VG_NAME");

  if (lv_name == NULL || vg_name == NULL)
    goto out;

  uuid = g_udev_device_get_sysfs_attr (device->priv->d, "dm/uuid");
  if (uuid == NULL || !g_str_has_prefix (uuid, "LVM-"))
    goto out;

  vg_uuid = extract_lvm_uuid (uuid + 4);
  if (vg_uuid == NULL)
    goto out;

  lv_uuid = extract_lvm_uuid (uuid + 4 + 32);
  if (lv_uuid == NULL)
    goto out;

  is_lv = TRUE;
  device_set_linux_lvm2_lv_name (device, lv_name);
  device_set_linux_lvm2_lv_uuid (device, lv_uuid);
  device_set_linux_lvm2_lv_group_name (device, vg_name);
  device_set_linux_lvm2_lv_group_uuid (device, vg_uuid);

  device_set_device_is_drive (device, FALSE);
  device_set_device_is_partition (device, FALSE);

 out:
  device_set_device_is_linux_lvm2_lv (device, is_lv);
  g_free (vg_uuid);
  g_free (lv_uuid);
  return TRUE;
}
