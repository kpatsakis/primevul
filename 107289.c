inf_gtk_certificate_manager_set_known_hosts(InfGtkCertificateManager* manager,
                                            const gchar* known_hosts_file)
{
  InfGtkCertificateManagerPrivate* priv;
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(manager);

  /* TODO: If there are running queries, the we need to load the new hosts
   * file and then change it in all queries. */
  g_assert(priv->queries == NULL);

  g_free(priv->known_hosts_file);
  priv->known_hosts_file = g_strdup(known_hosts_file);
}
