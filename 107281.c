inf_gtk_certificate_manager_get_property(GObject* object,
                                         guint prop_id,
                                         GValue* value,
                                         GParamSpec* pspec)
{
  InfGtkCertificateManager* manager;
  InfGtkCertificateManagerPrivate* priv;

  manager = INF_GTK_CERTIFICATE_MANAGER(object);
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(manager);

  switch(prop_id)
  {
  case PROP_PARENT_WINDOW:
    g_value_set_object(value, G_OBJECT(priv->parent_window));
    break;
  case PROP_XMPP_MANAGER:
    g_value_set_object(value, G_OBJECT(priv->xmpp_manager));
    break;
  case PROP_KNOWN_HOSTS_FILE:
    g_value_set_string(value, priv->known_hosts_file);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}
