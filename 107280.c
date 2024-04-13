inf_gtk_certificate_manager_finalize(GObject* object)
{
  InfGtkCertificateManager* manager;
  InfGtkCertificateManagerPrivate* priv;

  manager = INF_GTK_CERTIFICATE_MANAGER(object);
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(manager);

  inf_gtk_certificate_manager_set_known_hosts(manager, NULL);
  g_assert(priv->known_hosts_file == NULL);

  G_OBJECT_CLASS(inf_gtk_certificate_manager_parent_class)->finalize(object);
}
