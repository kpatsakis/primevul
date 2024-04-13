g_proxy_get_signal_match_rule (DBusGProxy *proxy)
{
  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);
  /* FIXME Escaping is required here */
  
  if (priv->name)
    return g_strdup_printf ("type='signal',sender='%s',path='%s',interface='%s'",
                            priv->name, priv->path, priv->interface);
  else
    return g_strdup_printf ("type='signal',path='%s',interface='%s'",
                            priv->path, priv->interface);
}
