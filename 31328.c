list_proxies_foreach (gpointer key,
                      gpointer value,
                      gpointer user_data)
{
  DBusGProxyList *list;
  GSList **ret;
  GSList *tmp;
  
  list = value;
  ret = user_data;

  tmp = list->proxies;
  while (tmp != NULL)
    {
      DBusGProxy *proxy = DBUS_G_PROXY (tmp->data);

      g_object_ref (proxy);
      *ret = g_slist_prepend (*ret, proxy);
      
      tmp = tmp->next;
    }
}
