tristring_from_proxy (DBusGProxy *proxy)
{
  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);

  return tristring_alloc_from_strings (0,
                                       priv->name,
                                       priv->path,
                                       priv->interface);
}
