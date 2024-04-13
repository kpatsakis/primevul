update_info_linux_lvm2_pv (Device *device)
{
  const gchar *pv_uuid;
  guint64 pv_num_mda;
  const gchar *vg_name;
  const gchar *vg_uuid;
  gboolean is_pv;
  guint64 vg_size;
  guint64 vg_unallocated_size;
  guint64 vg_extent_size;
  guint64 vg_seqnum;
  const gchar* const *vg_pvs;
  const gchar* const *vg_lvs;

  pv_uuid               = g_udev_device_get_property (device->priv->d, "UDISKS_LVM2_PV_UUID");
  pv_num_mda            = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_LVM2_PV_NUM_MDA");
  vg_name               = g_udev_device_get_property (device->priv->d, "UDISKS_LVM2_PV_VG_NAME");
  vg_uuid               = g_udev_device_get_property (device->priv->d, "UDISKS_LVM2_PV_VG_UUID");
  vg_size               = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_LVM2_PV_VG_SIZE");
  vg_unallocated_size   = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_LVM2_PV_VG_FREE_SIZE");
  vg_extent_size        = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_LVM2_PV_VG_EXTENT_SIZE");
  vg_seqnum             = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_LVM2_PV_VG_SEQNUM");
  vg_pvs                = g_udev_device_get_property_as_strv (device->priv->d, "UDISKS_LVM2_PV_VG_PV_LIST");
  vg_lvs                = g_udev_device_get_property_as_strv (device->priv->d, "UDISKS_LVM2_PV_VG_LV_LIST");

  is_pv = FALSE;

  if (pv_uuid == NULL)
    goto out;

  is_pv = TRUE;
  device_set_linux_lvm2_pv_uuid (device, pv_uuid);
  device_set_linux_lvm2_pv_num_metadata_areas (device, pv_num_mda);
  device_set_linux_lvm2_pv_group_name (device, vg_name);
  device_set_linux_lvm2_pv_group_uuid (device, vg_uuid);
  device_set_linux_lvm2_pv_group_size (device, vg_size);
  device_set_linux_lvm2_pv_group_unallocated_size (device, vg_unallocated_size);
  device_set_linux_lvm2_pv_group_extent_size (device, vg_extent_size);
  device_set_linux_lvm2_pv_group_sequence_number (device, vg_seqnum);
  device_set_linux_lvm2_pv_group_physical_volumes (device, (GStrv) vg_pvs);
  device_set_linux_lvm2_pv_group_logical_volumes (device, (GStrv) vg_lvs);

 out:
  device_set_device_is_linux_lvm2_pv (device, is_pv);
  return TRUE;
}
