inf_gtk_certificate_manager_load_known_hosts(InfGtkCertificateManager* mgr,
                                             GError** error)
{
  InfGtkCertificateManagerPrivate* priv;
  GHashTable* table;
  gchar* content;
  gsize size;
  GError* local_error;

  gchar* out_buf;
  gsize out_buf_len;
  gchar* pos;
  gchar* prev;
  gchar* next;
  gchar* sep;

  gsize len;
  gsize out_len;
  gint base64_state;
  guint base64_save;

  gnutls_datum_t data;
  gnutls_x509_crt_t cert;
  int res;

  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(mgr);

  table = g_hash_table_new_full(
    g_str_hash,
    g_str_equal,
    g_free,
    (GDestroyNotify)gnutls_x509_crt_deinit
  );

  local_error = NULL;
  g_file_get_contents(priv->known_hosts_file, &content, &size, &local_error);
  if(local_error != NULL)
  {
    if(local_error->domain == G_FILE_ERROR &&
       local_error->code == G_FILE_ERROR_NOENT)
    {
      return table;
    }

    g_propagate_prefixed_error(
      error,
      local_error,
      _("Failed to open known hosts file \"%s\": "),
      priv->known_hosts_file
    );

    g_hash_table_destroy(table);
    return NULL;
  }

  out_buf = NULL;
  out_buf_len = 0;
  prev = content;
  for(prev = content; prev != NULL; prev = next)
  {
    pos = strchr(prev, '\n');
    next = NULL;

    if(pos == NULL)
      pos = content + size;
    else
      next = pos + 1;

    sep = inf_gtk_certificate_manager_memrchr(prev, ':', pos - prev);
    if(sep == NULL) continue; /* ignore line */

    *sep = '\0';
    if(g_hash_table_lookup(table, prev) != NULL)
    {
      g_set_error(
        error,
        g_quark_from_static_string("INF_GTK_CERTIFICATE_MANAGER_ERROR"),
        INF_GTK_CERTIFICATE_MANAGER_ERROR_DUPLICATE_HOST_ENTRY,
        _("Certificate for host \"%s\" appears twice in "
          "known hosts file \"%s\""),
        prev,
        priv->known_hosts_file
      );

      g_hash_table_destroy(table);
      g_free(out_buf);
      g_free(content);
      return NULL;
    }

    /* decode base64, import DER certificate */
    len = (pos - (sep + 1));
    out_len = len * 3 / 4;

    if(out_len > out_buf_len)
    {
      out_buf = g_realloc(out_buf, out_len);
      out_buf_len = out_len;
    }

    base64_state = 0;
    base64_save = 0;

    out_len = g_base64_decode_step(
      sep + 1,
      len,
      out_buf,
      &base64_state,
      &base64_save
    );

    cert = NULL;
    res = gnutls_x509_crt_init(&cert);
    if(res == GNUTLS_E_SUCCESS)
    {
      data.data = out_buf;
      data.size = out_len;
      res = gnutls_x509_crt_import(cert, &data, GNUTLS_X509_FMT_DER);
    }

    if(res != GNUTLS_E_SUCCESS)
    {
      inf_gnutls_set_error(&local_error, res);

      g_propagate_prefixed_error(
        error,
        local_error,
        _("Failed to read certificate for host \"%s\" from "
          "known hosts file \"%s\": "),
        prev,
        priv->known_hosts_file
      );

      if(cert != NULL)
        gnutls_x509_crt_deinit(cert);

      g_hash_table_destroy(table);
      g_free(out_buf);
      g_free(content);
      return NULL;
    }

    g_hash_table_insert(table, g_strdup(prev), cert);
  }

  g_free(out_buf);
  g_free(content);
  return table;
}
