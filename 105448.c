device_local_is_busy (Device *device,
                      gboolean check_partitions,
                      GError **error)
{
  gboolean ret;

  ret = TRUE;

  /* busy if a job is pending */
  if (device->priv->job != NULL)
    {
      g_set_error (error, ERROR, ERROR_BUSY, "A job is pending on %s", device->priv->device_file);
      goto out;
    }

  /* or if we're mounted */
  if (device->priv->device_is_mounted)
    {
      g_set_error (error, ERROR, ERROR_BUSY, "%s is mounted", device->priv->device_file);
      goto out;
    }

  /* or if another block device is using/holding us (e.g. if holders/ is non-empty in sysfs) */
  if (device->priv->holders_objpath->len > 0)
    {
      if (device->priv->device_is_linux_dmmp)
        {
          /* This is OK */
        }
      else
        {
          g_set_error (error,
                       ERROR,
                       ERROR_BUSY,
                       "One or more block devices are holding %s",
                       device->priv->device_file);
          goto out;
        }
    }

  /* If we are an extended partition, we are also busy if one or more logical partitions are busy
   * even if @check_partitions is FALSE... This is because an extended partition only really is
   * a place holder.
   */
  if (g_strcmp0 (device->priv->partition_scheme, "mbr") == 0 && device->priv->partition_type != NULL)
    {
      gint partition_type;
      partition_type = strtol (device->priv->partition_type, NULL, 0);
      if (partition_type == 0x05 || partition_type == 0x0f || partition_type == 0x85)
        {
          Device *drive_device;
          drive_device = daemon_local_find_by_object_path (device->priv->daemon, device->priv->partition_slave);
          if (device_local_logical_partitions_are_busy (drive_device))
            {
              g_set_error (error, ERROR, ERROR_BUSY, "%s is an MS-DOS extended partition and one or more "
                           "logical partitions are busy", device->priv->device_file);
              goto out;
            }
        }
    }

  /* if we are a partition table, we are busy if one of our partitions are busy */
  if (check_partitions && device->priv->device_is_partition_table)
    {
      if (device_local_partitions_are_busy (device))
        {
          g_set_error (error, ERROR, ERROR_BUSY, "One or more partitions are busy on %s", device->priv->device_file);
          goto out;
        }
    }

  ret = FALSE;

 out:
  return ret;
}
