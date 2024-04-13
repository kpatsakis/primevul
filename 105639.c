update_info_partition_table (Device *device)
{
  gboolean is_partition_table;

  is_partition_table = FALSE;

  /* Check if udisks-part-id identified the device as a partition table.. this includes
   * identifying partition tables set up by kpartx for multipath etc.
   */
  if (g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_PARTITION_TABLE"))
    {
      device_set_partition_table_scheme (device,
                                         g_udev_device_get_property (device->priv->d,
                                                                     "UDISKS_PARTITION_TABLE_SCHEME"));
      device_set_partition_table_count (device,
                                        g_udev_device_get_property_as_int (device->priv->d,
                                                                           "UDISKS_PARTITION_TABLE_COUNT"));
      is_partition_table = TRUE;
    }

  /* Note that udisks-part-id might not detect all partition table
   * formats.. so in the negative case, also double check with
   * information in sysfs.
   *
   * The kernel guarantees that all childs are created before the
   * uevent for the parent is created. So if we have childs, we must
   * be a partition table.
   *
   * To detect a child we check for the existance of a subdir that has
   * the parents name as a prefix (e.g. for parent sda then sda1,
   * sda2, sda3 ditto md0, md0p1 etc. etc. will work).
   */
  if (!is_partition_table)
    {
      gchar *s;
      GDir *dir;

      s = g_path_get_basename (device->priv->native_path);
      if ((dir = g_dir_open (device->priv->native_path, 0, NULL)) != NULL)
        {
          guint partition_count;
          const gchar *name;

          partition_count = 0;
          while ((name = g_dir_read_name (dir)) != NULL)
            {
              if (g_str_has_prefix (name, s))
                {
                  partition_count++;
                }
            }
          g_dir_close (dir);

          if (partition_count > 0)
            {
              device_set_partition_table_scheme (device, "");
              device_set_partition_table_count (device, partition_count);
              is_partition_table = TRUE;
            }
        }
      g_free (s);
    }

  device_set_device_is_partition_table (device, is_partition_table);
  if (!is_partition_table)
    {
      /* otherwise, clear all the data */
      device_set_partition_table_scheme (device, NULL);
      device_set_partition_table_count (device, 0);
    }

  return TRUE;
}
