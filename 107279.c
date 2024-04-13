inf_gtk_certificate_manager_dispose(GObject* object)
{
  InfGtkCertificateManager* manager;
  InfGtkCertificateManagerPrivate* priv;
  GSList* item;

  manager = INF_GTK_CERTIFICATE_MANAGER(object);
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(manager);

  if(priv->parent_window != NULL)
  {
    g_object_unref(priv->parent_window);
    priv->parent_window = NULL;
  }

  if(priv->xmpp_manager != NULL)
  {
    g_object_unref(priv->xmpp_manager);
    priv->xmpp_manager = NULL;
  }

  for(item = priv->queries; item != NULL; item = g_slist_next(item))
  {
    inf_gtk_certificate_manager_query_free(
      (InfGtkCertificateManagerQuery*)item->data
    );
  }

  g_slist_free(priv->queries);
  priv->queries = NULL;

  G_OBJECT_CLASS(inf_gtk_certificate_manager_parent_class)->dispose(object);
}
