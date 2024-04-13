update_info_partition (Device *device)
{
  gboolean is_partition;

  is_partition = FALSE;

  /* Check if udisks-part-id identified the device as a partition.. this includes
   * identifying partitions set up by kpartx for multipath
   */
  if (g_udev_device_has_property (device->priv->d, "UDISKS_PARTITION"))
    {
      guint64 size;
      const gchar *scheme;
      const gchar *type;
      const gchar *label;
      const gchar *uuid;
      const gchar* const *flags;
      guint64 offset;
      guint64 alignment_offset;
      const gchar *slave_sysfs_path;
      gint number;

      scheme = g_udev_device_get_property (device->priv->d, "UDISKS_PARTITION_SCHEME");
      size = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_PARTITION_SIZE");
      type = g_udev_device_get_property (device->priv->d, "UDISKS_PARTITION_TYPE");
      label = g_udev_device_get_property (device->priv->d, "UDISKS_PARTITION_LABEL");
      uuid = g_udev_device_get_property (device->priv->d, "UDISKS_PARTITION_UUID");
      flags = g_udev_device_get_property_as_strv (device->priv->d, "UDISKS_PARTITION_FLAGS");
      offset = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_PARTITION_OFFSET");
      alignment_offset = g_udev_device_get_property_as_uint64 (device->priv->d, "UDISKS_PARTITION_ALIGNMENT_OFFSET");
      number = g_udev_device_get_property_as_int (device->priv->d, "UDISKS_PARTITION_NUMBER");
      slave_sysfs_path = g_udev_device_get_property (device->priv->d, "UDISKS_PARTITION_SLAVE");

      if (slave_sysfs_path != NULL && scheme != NULL && number > 0)
        {
          gchar *s;

          device_set_partition_scheme (device, scheme);
          device_set_partition_size (device, size);
          device_set_partition_type (device, type);
          device_set_partition_label (device, label);
          device_set_partition_uuid (device, uuid);
          device_set_partition_flags (device, (gchar **) flags);
          device_set_partition_offset (device, offset);
          device_set_partition_alignment_offset (device, alignment_offset);
          device_set_partition_number (device, number);

          s = compute_object_path (slave_sysfs_path);
          device_set_partition_slave (device, s);
          g_free (s);

          is_partition = TRUE;
        }
    }

  /* Also handle the case where we are partitioned by the kernel and don't have
   * any UDISKS_PARTITION_* properties.
   *
   * This works without any udev UDISKS_PARTITION_* properties and is
   * there for maximum compatibility since udisks-part-id only knows a
   * limited set of partition table formats.
   */
  if (!is_partition && sysfs_file_exists (device->priv->native_path, "start"))
    {
      guint64 start;
      guint64 size;
      guint64 offset;
      guint64 alignment_offset;
      gchar *s;
      guint n;

      device_set_device_is_partition (device, TRUE);
      start = sysfs_get_uint64 (device->priv->native_path, "start");
      size = sysfs_get_uint64 (device->priv->native_path, "size");
      alignment_offset = sysfs_get_uint64 (device->priv->native_path, "alignment_offset");

      device_set_partition_size (device, size * 512); /* device->priv->device_block_size; */
      device_set_partition_alignment_offset (device, alignment_offset);

      offset = sysfs_get_uint64 (device->priv->native_path, "start") * device->priv->device_block_size;
      device_set_partition_offset (device, offset);

      s = device->priv->native_path;
      for (n = strlen (s) - 1; n >= 0 && g_ascii_isdigit (s[n]); n--)
        ;
      device_set_partition_number (device, strtol (s + n + 1, NULL, 0));

      s = g_strdup (device->priv->native_path);
      for (n = strlen (s) - 1; n >= 0 && s[n] != '/'; n--)
        s[n] = '\0';
      s[n] = '\0';
      device_set_partition_slave (device, compute_object_path (s));
      g_free (s);

      is_partition = TRUE;
    }

  device_set_device_is_partition (device, is_partition);
  if (!is_partition)
    {
      /* otherwise, clear all the data */
      device_set_partition_scheme (device, NULL);
      device_set_partition_size (device, device->priv->device_size);
      device_set_partition_type (device, NULL);
      device_set_partition_label (device, NULL);
      device_set_partition_uuid (device, NULL);
      device_set_partition_flags (device, NULL);
    }
  else
    {
      device_set_device_is_drive (device, FALSE);
    }

  return TRUE;
}
