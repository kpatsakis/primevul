dbus_g_proxy_manager_list_all (DBusGProxyManager *manager)
{
  GSList *ret;

  ret = NULL;

  if (manager->proxy_lists)
    {
      g_hash_table_foreach (manager->proxy_lists,
                            list_proxies_foreach,
                            &ret);
    }

  return ret;
}
