update_info_is_system_internal (Device *device)
{
  gboolean is_system_internal;

  /* TODO: make it possible to override this property from a udev property.
   */

  /* start out by assuming the device is system internal, then adjust depending on what kind of
   * device we are dealing with
   */
  is_system_internal = TRUE;

  /* A Linux MD device is system internal if, and only if
   *
   * - a single component is system internal
   * - there are no components
   */
  if (device->priv->device_is_linux_md)
    {
      is_system_internal = FALSE;

      if (device->priv->slaves_objpath->len == 0)
        {
          is_system_internal = TRUE;
        }
      else
        {
          guint n;

          for (n = 0; n < device->priv->slaves_objpath->len; n++)
            {
              const gchar *slave_objpath;
              Device *slave;

              slave_objpath = device->priv->slaves_objpath->pdata[n];
              slave = daemon_local_find_by_object_path (device->priv->daemon, slave_objpath);
              if (slave == NULL)
                continue;

              if (slave->priv->device_is_system_internal)
                {
                  is_system_internal = TRUE;
                  break;
                }
            }
        }

      goto determined;
    }

  /* a partition is system internal only if the drive it belongs to is system internal */
  if (device->priv->device_is_partition)
    {
      Device *enclosing_device;

      enclosing_device = daemon_local_find_by_object_path (device->priv->daemon, device->priv->partition_slave);
      if (enclosing_device != NULL)
        {
          is_system_internal = enclosing_device->priv->device_is_system_internal;
        }
      else
        {
          is_system_internal = TRUE;
        }

      goto determined;
    }

  /* a LUKS cleartext device is system internal only if the underlying crypto-text
   * device is system internal
   */
  if (device->priv->device_is_luks_cleartext)
    {
      Device *enclosing_device;
      enclosing_device = daemon_local_find_by_object_path (device->priv->daemon, device->priv->luks_cleartext_slave);
      if (enclosing_device != NULL)
        {
          is_system_internal = enclosing_device->priv->device_is_system_internal;
        }
      else
        {
          is_system_internal = TRUE;
        }

      goto determined;
    }

  /* devices with removable media are never system internal */
  if (device->priv->device_is_removable)
    {
      is_system_internal = FALSE;
      goto determined;
    }

  /* devices on certain buses are never system internal */
  if (device->priv->device_is_drive && device->priv->drive_connection_interface != NULL)
    {

      if (strcmp (device->priv->drive_connection_interface, "ata_serial_esata") == 0
          || strcmp (device->priv->drive_connection_interface, "sdio") == 0
          || strcmp (device->priv->drive_connection_interface, "usb") == 0
          || strcmp (device->priv->drive_connection_interface, "firewire") == 0)
        {
          is_system_internal = FALSE;
        }
      else
        {
          is_system_internal = TRUE;
        }
      goto determined;
    }

 determined:
  device_set_device_is_system_internal (device, is_system_internal);

  return TRUE;
}
