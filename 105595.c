register_disks_device (Device *device)
{
  DBusConnection *connection;
  GError *error = NULL;

  device->priv->system_bus_connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
  if (device->priv->system_bus_connection == NULL)
    {
      if (error != NULL)
        {
          g_critical ("error getting system bus: %s", error->message);
          g_error_free (error);
        }
      goto error;
    }
  connection = dbus_g_connection_get_connection (device->priv->system_bus_connection);

  device->priv->object_path = compute_object_path (device->priv->native_path);

  /* safety first */
  if (dbus_g_connection_lookup_g_object (device->priv->system_bus_connection, device->priv->object_path) != NULL)
    {
      g_error ("**** HACK: Wanting to register object at path `%s' but there is already an "
               "object there. This is an internal error in the daemon. Aborting.\n", device->priv->object_path);
    }

  dbus_g_connection_register_g_object (device->priv->system_bus_connection,
                                       device->priv->object_path,
                                       G_OBJECT (device));

  return TRUE;

 error:
  return FALSE;
}
