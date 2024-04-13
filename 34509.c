on_name_owner_changed_signal (GDBusConnection *connection,
                              const gchar     *sender_name,
                              const gchar     *object_path,
                              const gchar     *interface_name,
                              const gchar     *signal_name,
                              GVariant        *parameters,
                              gpointer         user_data)
{
  PolkitBackendInteractiveAuthority *authority = POLKIT_BACKEND_INTERACTIVE_AUTHORITY (user_data);
  const gchar *name;
  const gchar *old_owner;
  const gchar *new_owner;

  g_variant_get (parameters,
                 "(&s&s&s)",
                 &name,
                 &old_owner,
                 &new_owner);

  polkit_backend_interactive_authority_system_bus_name_owner_changed (authority,
                                                                      name,
                                                                      old_owner,
                                                                      new_owner);
}
