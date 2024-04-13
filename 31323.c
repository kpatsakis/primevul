g_proxy_list_new (DBusGProxy *first_proxy)
{
  DBusGProxyList *list;
  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(first_proxy);
  
  list = (void*) tristring_alloc_from_strings (G_STRUCT_OFFSET (DBusGProxyList, name),
                                               priv->name,
                                               priv->path,
                                               priv->interface);
  list->proxies = NULL;

  return list;
}
