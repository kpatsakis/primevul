inf_gtk_certificate_manager_notify_status_cb(GObject* object,
                                             GParamSpec* pspec,
                                             gpointer user_data)
{
  InfGtkCertificateManagerQuery* query;
  InfGtkCertificateManagerPrivate* priv;
  InfXmppConnection* connection;
  InfXmlConnectionStatus status;

  query = (InfGtkCertificateManagerQuery*)user_data;
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(query->manager);
  connection = INF_XMPP_CONNECTION(object);

  g_object_get(G_OBJECT(connection), "status", &status, NULL);

  if(status == INF_XML_CONNECTION_CLOSING ||
     status == INF_XML_CONNECTION_CLOSED)
  {
    priv->queries = g_slist_remove(priv->queries, query);
    inf_gtk_certificate_manager_query_free(query);
  }
}
