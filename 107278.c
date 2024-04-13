inf_gtk_certificate_manager_connection_added_cb(InfXmppManager* manager,
                                                InfXmppConnection* connection,
                                                gpointer user_data)
{
  InfXmppConnectionSite site;
  g_object_get(G_OBJECT(connection), "site", &site, NULL);

  if(site == INF_XMPP_CONNECTION_CLIENT)
  {
    inf_xmpp_connection_set_certificate_callback(
      connection,
      GNUTLS_CERT_REQUIRE, /* require a server certificate */
      inf_gtk_certificate_manager_certificate_func,
      user_data,
      NULL
    );
  }
}
