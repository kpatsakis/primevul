inf_gtk_certificate_manager_new(GtkWindow* parent_window,
                                InfXmppManager* xmpp_manager,
                                const gchar* known_hosts_file)
{
  GObject* object;

  object = g_object_new(
    INF_GTK_TYPE_CERTIFICATE_MANAGER,
    "parent-window", parent_window,
    "xmpp-manager", xmpp_manager,
    "known-hosts-file", known_hosts_file,
    NULL
  );

  return INF_GTK_CERTIFICATE_MANAGER(object);
}
