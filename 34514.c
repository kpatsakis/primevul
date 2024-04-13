polkit_backend_interactive_authority_finalize (GObject *object)
{
  PolkitBackendInteractiveAuthority *interactive_authority;
  PolkitBackendInteractiveAuthorityPrivate *priv;

  interactive_authority = POLKIT_BACKEND_INTERACTIVE_AUTHORITY (object);
  priv = POLKIT_BACKEND_INTERACTIVE_AUTHORITY_GET_PRIVATE (interactive_authority);

  if (priv->name_owner_changed_signal_id > 0)
    g_dbus_connection_signal_unsubscribe (priv->system_bus_connection, priv->name_owner_changed_signal_id);

  if (priv->system_bus_connection != NULL)
    g_object_unref (priv->system_bus_connection);

  if (priv->action_pool != NULL)
    g_object_unref (priv->action_pool);

  if (priv->session_monitor != NULL)
    g_object_unref (priv->session_monitor);

  temporary_authorization_store_free (priv->temporary_authorization_store);

  g_hash_table_unref (priv->hash_scope_to_authentication_agent);

  G_OBJECT_CLASS (polkit_backend_interactive_authority_parent_class)->finalize (object);
}
