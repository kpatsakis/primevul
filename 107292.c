inf_gtk_certificate_manager_write_known_hosts(InfGtkCertificateManager* mgr,
                                              GHashTable* table,
                                              GError** error)
{
  InfGtkCertificateManagerPrivate* priv;
  gchar* dirname;
  GIOChannel* channel;
  GIOStatus status;

  GHashTableIter iter;
  gpointer key;
  gpointer value;
  const gchar* hostname;
  gnutls_x509_crt_t cert;

  size_t size;
  int res;
  gchar* buffer;
  gchar* encoded_cert;

  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(mgr);
  
  /* Note that we pin the whole certificate and not only the public key of
   * our known hosts. This allows us to differentiate two cases when a
   * host presents a new certificate:
   *    a) The old certificate has expired or is very close to expiration. In
   *       this case we still show a message to the user asking whether they
   *       trust the new certificate.
   *    b) The old certificate was perfectly valid. In this case we show a
   *       message saying that the certificate change was unexpected, and
   *       unless it was expected the host should not be trusted.
   */
  dirname = g_path_get_dirname(priv->known_hosts_file);
  if(!inf_file_util_create_directory(dirname, 0755, error))
  {
    g_free(dirname);
    return FALSE;
  }

  g_free(dirname);

  channel = g_io_channel_new_file(priv->known_hosts_file, "w", error);
  if(channel == NULL) return FALSE;

  status = g_io_channel_set_encoding(channel, NULL, error);
  if(status != G_IO_STATUS_NORMAL)
  {
    g_io_channel_unref(channel);
    return FALSE;
  }

  g_hash_table_iter_init(&iter, table);
  while(g_hash_table_iter_next(&iter, &key, &value))
  {
    hostname = (const gchar*)key;
    cert = (gnutls_x509_crt_t)value;

    size = 0;
    res = gnutls_x509_crt_export(cert, GNUTLS_X509_FMT_DER, NULL, &size);
    g_assert(res != GNUTLS_E_SUCCESS);

    buffer = NULL;
    if(res == GNUTLS_E_SHORT_MEMORY_BUFFER)
    {
      buffer = g_malloc(size);
      res = gnutls_x509_crt_export(cert, GNUTLS_X509_FMT_DER, buffer, &size);
    }

    if(res != GNUTLS_E_SUCCESS)
    {
      g_free(buffer);
      g_io_channel_unref(channel);
      inf_gnutls_set_error(error, res);
      return FALSE;
    }

    encoded_cert = g_base64_encode(buffer, size);
    g_free(buffer);

    status = g_io_channel_write_chars(channel, hostname, strlen(hostname), NULL, error);
    if(status == G_IO_STATUS_NORMAL)
      status = g_io_channel_write_chars(channel, ":", 1, NULL, error);
    if(status == G_IO_STATUS_NORMAL)
      status = g_io_channel_write_chars(channel, encoded_cert, strlen(encoded_cert), NULL, error);
    if(status == G_IO_STATUS_NORMAL)
      status = g_io_channel_write_chars(channel, "\n", 1, NULL, error);

    g_free(encoded_cert);

    if(status != G_IO_STATUS_NORMAL)
    {
      g_io_channel_unref(channel);
      return FALSE;
    }
  }

  g_io_channel_unref(channel);
  return TRUE;
}
