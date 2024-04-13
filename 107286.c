inf_gtk_certificate_manager_query_free(InfGtkCertificateManagerQuery* query)
{
  inf_signal_handlers_disconnect_by_func(
    G_OBJECT(query->connection),
    G_CALLBACK(inf_gtk_certificate_manager_notify_status_cb),
    query
  );

  g_object_unref(query->connection);
  inf_certificate_chain_unref(query->certificate_chain);
  gtk_widget_destroy(GTK_WIDGET(query->dialog));
  g_hash_table_unref(query->known_hosts);
  g_slice_free(InfGtkCertificateManagerQuery, query);
}
