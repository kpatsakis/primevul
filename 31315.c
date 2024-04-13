dbus_g_proxy_manager_get (DBusConnection *connection)
{
  DBusGProxyManager *manager;

  dbus_connection_allocate_data_slot (&g_proxy_manager_slot);
  if (g_proxy_manager_slot < 0)
    g_error ("out of memory");
  
  g_static_mutex_lock (&connection_g_proxy_lock);
  
  manager = dbus_connection_get_data (connection, g_proxy_manager_slot);
  if (manager != NULL)
    {
      dbus_connection_free_data_slot (&g_proxy_manager_slot);
      dbus_g_proxy_manager_ref (manager);
      g_static_mutex_unlock (&connection_g_proxy_lock);
      return manager;
    }
  
  manager = g_new0 (DBusGProxyManager, 1);

  manager->refcount = 1;
  manager->connection = connection;

  g_static_mutex_init (&manager->lock);

  /* Proxy managers keep the connection alive, which means that
   * DBusGProxy indirectly does. To free a connection you have to free
   * all the proxies referring to it.
   */
  dbus_connection_ref (manager->connection);

  dbus_connection_set_data (connection, g_proxy_manager_slot,
                            manager, NULL);

  dbus_connection_add_filter (connection, dbus_g_proxy_manager_filter,
                              manager, NULL);
  
  g_static_mutex_unlock (&connection_g_proxy_lock);
  
  return manager;
}
