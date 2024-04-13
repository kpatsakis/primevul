inf_gtk_certificate_manager_response_cb(GtkDialog* dialog,
                                        int response_id,
                                        gpointer user_data)
{
  InfGtkCertificateManagerQuery* query;
  InfGtkCertificateManagerPrivate* priv;
  InfXmppConnection* connection;

  gchar* hostname;
  gnutls_x509_crt_t cert;
  gnutls_x509_crt_t known_cert;
  GError* error;
  gboolean cert_equal;

  query = (InfGtkCertificateManagerQuery*)user_data;
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(query->manager);

  connection = query->connection;
  g_object_ref(connection);

  switch(response_id)
  {
  case GTK_RESPONSE_ACCEPT:
    g_object_get(
      G_OBJECT(query->connection),
      "remote-hostname", &hostname,
      NULL
    );

    /* Add the certificate to the known hosts file, but only if it is not
     * already, to avoid unnecessary disk I/O. */
    cert =
      inf_certificate_chain_get_own_certificate(query->certificate_chain);
    known_cert = g_hash_table_lookup(query->known_hosts, hostname);

    error = NULL;
    cert_equal = FALSE;
    if(known_cert != NULL)
    {
      cert_equal = inf_gtk_certificate_manager_compare_fingerprint(
        cert,
        known_cert,
        &error
      );
    }

    if(error != NULL)
    {
      g_warning(
        _("Failed to add certificate to list of known hosts: %s"),
        error->message
      );
    }
    else if(!cert_equal)
    {
      cert = inf_cert_util_copy_certificate(cert, &error);
      g_hash_table_insert(query->known_hosts, hostname, cert);

      inf_gtk_certificate_manager_write_known_hosts_with_warning(
        query->manager,
        query->known_hosts
      );
    }
    else
    {
      g_free(hostname);
    }

    priv->queries = g_slist_remove(priv->queries, query);
    inf_gtk_certificate_manager_query_free(query);

    inf_xmpp_connection_certificate_verify_continue(connection);
    break;
  case GTK_RESPONSE_REJECT:
  case GTK_RESPONSE_DELETE_EVENT:
    priv->queries = g_slist_remove(priv->queries, query);
    inf_gtk_certificate_manager_query_free(query);
    inf_xmpp_connection_certificate_verify_cancel(connection, NULL);
    break;
  default:
    g_assert_not_reached();
    break;
  }

  g_object_unref(connection);
}
