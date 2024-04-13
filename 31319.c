dbus_g_proxy_manager_unmonitor_name_owner (DBusGProxyManager  *manager,
					   const char         *name)
{
  DBusGProxyNameOwnerInfo *info;
  const char *owner;
  gboolean ret;

  ret = dbus_g_proxy_manager_lookup_name_owner (manager, name, &info, &owner);
  g_assert (ret);
  g_assert (info != NULL);
  g_assert (owner != NULL);

  info->refcount--;
  if (info->refcount == 0)
    {
      GSList *names;
      GSList *link;

      names = g_hash_table_lookup (manager->owner_names, owner);
      link = g_slist_find_custom (names, name, find_name_in_info);
      names = g_slist_delete_link (names, link);
      if (names != NULL)
	g_hash_table_insert (manager->owner_names, g_strdup (owner), names);
      else
	g_hash_table_remove (manager->owner_names, owner);

      g_free (info->name);
      g_free (info);
    }
}
