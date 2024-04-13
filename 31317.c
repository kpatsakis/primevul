dbus_g_proxy_manager_monitor_name_owner (DBusGProxyManager  *manager,
					 const char         *owner,
					 const char         *name)
{
  GSList *names;
  GSList *link;
  DBusGProxyNameOwnerInfo *nameinfo;

  names = g_hash_table_lookup (manager->owner_names, owner);
  link = g_slist_find_custom (names, name, find_name_in_info);
  
  if (!link)
    {
      nameinfo = g_new0 (DBusGProxyNameOwnerInfo, 1);
      nameinfo->name = g_strdup (name);
      nameinfo->refcount = 1;

      insert_nameinfo (manager, owner, nameinfo);
    }
  else
    {
      nameinfo = link->data;
      nameinfo->refcount++;
    }
}
