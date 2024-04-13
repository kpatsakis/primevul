inf_gtk_certificate_manager_write_known_hosts_with_warning(
  InfGtkCertificateManager* mgr,
  GHashTable* table)
{
  InfGtkCertificateManagerPrivate* priv;
  GError* error;
  gboolean result;

  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(mgr);
  error = NULL;

  result = inf_gtk_certificate_manager_write_known_hosts(
    mgr,
    table,
    &error
  );

  if(error != NULL)
  {
    g_warning(
      _("Failed to write file with known hosts \"%s\": %s"),
      priv->known_hosts_file,
      error->message
    );

    g_error_free(error);
  }
}
