device_removed (Device *device)
{
  guint n;

  device->priv->removed = TRUE;

  dbus_g_connection_unregister_g_object (device->priv->system_bus_connection, G_OBJECT (device));
  g_assert (dbus_g_connection_lookup_g_object (device->priv->system_bus_connection, device->priv->object_path) == NULL);

  /* device is now removed; update all slaves and holders */
  for (n = 0; n < device->priv->slaves_objpath->len; n++)
    {
      const gchar *objpath2 = ((gchar **) device->priv->slaves_objpath->pdata)[n];
      Device *device2;

      device2 = daemon_local_find_by_object_path (device->priv->daemon, objpath2);
      if (device2 != NULL)
        {
          update_info (device2);
        }
    }
  for (n = 0; n < device->priv->holders_objpath->len; n++)
    {
      const gchar *objpath2 = ((gchar **) device->priv->holders_objpath->pdata)[n];
      Device *device2;

      device2 = daemon_local_find_by_object_path (device->priv->daemon, objpath2);
      if (device2 != NULL)
        {
          update_info (device2);
        }
    }

  /* If the device is busy, we possibly need to clean up if the
   * device itself is busy. This includes
   *
   *  - force unmounting the device and/or all it's partitions
   *
   *  - tearing down a luks mapping if it's a cleartext device
   *    backed by a crypted device
   *
   * but see force_removal() for details.
   *
   * This is the normally the path where the enclosing device is
   * removed. Compare with device_changed() for the
   * other path.
   */
  force_removal (device, NULL, NULL);
}
