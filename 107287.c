inf_gtk_certificate_manager_ref_known_hosts(InfGtkCertificateManager* mgr,
                                            GError** error)
{
  InfGtkCertificateManagerPrivate* priv;
  InfGtkCertificateManagerQuery* query;

  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(mgr);
  if(priv->queries != NULL)
  {
    query = (InfGtkCertificateManagerQuery*)priv->queries->data;
    g_hash_table_ref(query->known_hosts);
    return query->known_hosts;
  }
  else
  {
    return inf_gtk_certificate_manager_load_known_hosts(mgr, error);
  }
}
