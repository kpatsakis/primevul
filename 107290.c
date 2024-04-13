inf_gtk_certificate_manager_set_property(GObject* object,
                                         guint prop_id,
                                         const GValue* value,
                                         GParamSpec* pspec)
{
  InfGtkCertificateManager* manager;
  InfGtkCertificateManagerPrivate* priv;

  manager = INF_GTK_CERTIFICATE_MANAGER(object);
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(manager);

  switch(prop_id)
  {
  case PROP_PARENT_WINDOW:
    g_assert(priv->parent_window == NULL); /* construct/only */
    priv->parent_window = GTK_WINDOW(g_value_dup_object(value));
    break;
  case PROP_XMPP_MANAGER:
    g_assert(priv->xmpp_manager == NULL); /* construct/only */
    priv->xmpp_manager = INF_XMPP_MANAGER(g_value_dup_object(value));

    g_signal_connect(
      G_OBJECT(priv->xmpp_manager),
      "connection-added",
      G_CALLBACK(inf_gtk_certificate_manager_connection_added_cb),
      manager
    );

    break;
  case PROP_KNOWN_HOSTS_FILE:
    inf_gtk_certificate_manager_set_known_hosts(
      manager,
      g_value_get_string(value)
    );

    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}
