dbus_g_proxy_manager_register (DBusGProxyManager *manager,
                               DBusGProxy        *proxy)
{
  DBusGProxyList *list;
  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);

  LOCK_MANAGER (manager);

  if (manager->proxy_lists == NULL)
    {
      g_assert (manager->owner_names == NULL);
      g_assert (manager->owner_match_rules == NULL);

      list = NULL;
      manager->proxy_lists = g_hash_table_new_full (tristring_hash,
                                                    tristring_equal,
                                                    NULL,
                                                    (GFreeFunc) g_proxy_list_free);
      manager->owner_names = g_hash_table_new_full (g_str_hash,
                                                    g_str_equal,
                                                    g_free,
                                                    NULL);
      manager->owner_match_rules = g_hash_table_new_full (g_str_hash,
                                                          g_str_equal,
                                                          g_free,
                                                          guint_slice_free);
    }
  else
    {
      char *tri;

      tri = tristring_from_proxy (proxy);
      
      list = g_hash_table_lookup (manager->proxy_lists, tri);

      g_free (tri);
    }
      
  if (list == NULL)
    {
      list = g_proxy_list_new (proxy);
      
      g_hash_table_replace (manager->proxy_lists,
                            list->name, list);
    }

  if (list->proxies == NULL && priv->name)
    {
      /* We have to add match rules to the server,
       * but only if the server is a message bus,
       * not if it's a peer.
       */
      char *rule;
      guint *refcount;

      rule = g_proxy_get_signal_match_rule (proxy);
      /* We don't check for errors; it's not like anyone would handle them, and
       * we don't want a round trip here.
       */
      dbus_bus_add_match (manager->connection, rule, NULL);
      g_free (rule);
       
      refcount = g_hash_table_lookup (manager->owner_match_rules, priv->name);

      if (refcount != NULL)
        {
          g_assert (*refcount != 0);
          g_assert (*refcount < G_MAXUINT);
          (*refcount)++;
        }
      else
        {
          char *rule;
          rule = get_owner_match_rule (priv->name);
          dbus_bus_add_match (manager->connection,
                              rule, NULL);
          g_free (rule);

          refcount = g_slice_new (guint);
          *refcount = 1;
          g_hash_table_insert (manager->owner_match_rules,
                               g_strdup (priv->name), refcount);
        }
    }

  g_assert (g_slist_find (list->proxies, proxy) == NULL);
  
  list->proxies = g_slist_prepend (list->proxies, proxy);

  if (!priv->for_owner)
    {
      const char *owner;
      DBusGProxyNameOwnerInfo *info;

      if (!dbus_g_proxy_manager_lookup_name_owner (manager, priv->name, &info, &owner))
	{
	  priv->name_call = manager_begin_bus_call (manager, "GetNameOwner",
						     got_name_owner_cb,
						     proxy, NULL,
						     G_TYPE_STRING,
						     priv->name, 
						     G_TYPE_INVALID);
	  
	  priv->associated = FALSE;
	}
      else
	{
	  info->refcount++;
	  priv->associated = TRUE;
	}
    }
  
  UNLOCK_MANAGER (manager);
}
