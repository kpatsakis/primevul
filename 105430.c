device_init (Device *device)
{
  device->priv = DEVICE_GET_PRIVATE (device);

  device->priv->device_file_by_id = g_ptr_array_new ();
  device->priv->device_file_by_path = g_ptr_array_new ();
  device->priv->device_mount_paths = g_ptr_array_new ();
  device->priv->partition_flags = g_ptr_array_new ();
  device->priv->drive_media_compatibility = g_ptr_array_new ();
  device->priv->drive_ports = g_ptr_array_new ();
  device->priv->drive_similar_devices = g_ptr_array_new ();
  device->priv->linux_md_component_state = g_ptr_array_new ();
  device->priv->linux_md_slaves = g_ptr_array_new ();
  device->priv->linux_lvm2_pv_group_physical_volumes = g_ptr_array_new ();
  device->priv->linux_lvm2_pv_group_logical_volumes = g_ptr_array_new ();
  device->priv->linux_dmmp_slaves = g_ptr_array_new ();

  device->priv->slaves_objpath = g_ptr_array_new ();
  device->priv->holders_objpath = g_ptr_array_new ();

  device->priv->drive_ata_smart_status = -1;
}
