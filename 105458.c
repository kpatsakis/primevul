device_new (Daemon *daemon,
            GUdevDevice *d)
{
  Device *device;
  const char *native_path;

  device = NULL;
  native_path = g_udev_device_get_sysfs_path (d);

  /* ignore ram devices */
  if (g_str_has_prefix (native_path, "/sys/devices/virtual/block/ram"))
    goto out;

  PROFILE ("device_new(native_path=%s): start", native_path);

  device = DEVICE (g_object_new (TYPE_DEVICE, NULL));
  device->priv->d = g_object_ref (d);
  device->priv->daemon = g_object_ref (daemon);
  device->priv->native_path = g_strdup (native_path);

  /* TODO: we might want to get this from udev or the kernel... to get the time when the device
   *       was initially detected... as opposed to this value which is when the device was detected
   *       by our daemon... but this will do for now...
   */
  device->priv->device_detection_time = (guint64) time (NULL);

  PROFILE ("device_new(native_path=%s): update_info", native_path);
  if (!update_info (device))
    {
      g_object_unref (device);
      device = NULL;
      goto out;
    }

  PROFILE ("device_new(native_path=%s): register_disks_device", native_path);
  if (!register_disks_device (DEVICE (device)))
    {
      g_object_unref (device);
      device = NULL;
      goto out;
    }

  /* if just added, update the smart data if applicable */
  if (device->priv->drive_ata_smart_is_available)
    {
      PROFILE ("device_new(native_path=%s): refresh ATA SMART DATA", native_path);
      gchar *ata_smart_refresh_data_options[] = { NULL };
      device_drive_ata_smart_refresh_data (device, ata_smart_refresh_data_options, NULL);
    }

  PROFILE ("device_new(native_path=%s): end", native_path);
 out:
  return device;
}
