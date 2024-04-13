dbus_g_proxy_manager_unref (DBusGProxyManager *manager)
{
  g_assert (manager != NULL);
  g_assert (manager->refcount > 0);

  LOCK_MANAGER (manager);
  manager->refcount -= 1;
  if (manager->refcount == 0)
    {
      UNLOCK_MANAGER (manager);

      if (manager->bus_proxy)
	g_object_unref (manager->bus_proxy);

      if (manager->proxy_lists)
        {
          /* can't have any proxies left since they hold
           * a reference to the proxy manager.
           */
          g_assert (g_hash_table_size (manager->proxy_lists) == 0);
          
          g_hash_table_destroy (manager->proxy_lists);
          manager->proxy_lists = NULL;

        }

      if (manager->owner_match_rules)
        {
	  /* Since we destroyed all proxies, none can be tracking
	   * name owners
	   */
          g_assert (g_hash_table_size (manager->owner_match_rules) == 0);
          g_hash_table_destroy (manager->owner_match_rules);
          manager->owner_match_rules = NULL;
        }

      if (manager->owner_names)
	{
	  /* Since we destroyed all proxies, none can be tracking
	   * name owners
	   */
          g_assert (g_hash_table_size (manager->owner_names) == 0);

	  g_hash_table_destroy (manager->owner_names);
	  manager->owner_names = NULL;
	}

      g_assert (manager->unassociated_proxies == NULL);
      
      g_static_mutex_free (&manager->lock);

      g_static_mutex_lock (&connection_g_proxy_lock);

      dbus_connection_remove_filter (manager->connection, dbus_g_proxy_manager_filter,
                                     manager);
      
      dbus_connection_set_data (manager->connection,
                                g_proxy_manager_slot,
                                NULL, NULL);

      g_static_mutex_unlock (&connection_g_proxy_lock);
      
      dbus_connection_unref (manager->connection);
      g_free (manager);

      dbus_connection_free_data_slot (&g_proxy_manager_slot);
    }
  else
    {
      UNLOCK_MANAGER (manager);
    }
}
