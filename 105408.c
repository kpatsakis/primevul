device_drive_inhibit_polling_authorized_cb (Daemon *daemon,
                                            Device *device,
                                            DBusGMethodInvocation *context,
                                            const gchar *action_id,
                                            guint num_user_data,
                                            gpointer *user_data_elements)
{
  gchar **options = user_data_elements[0];
  Inhibitor *inhibitor;
  guint n;

  for (n = 0; options[n] != NULL; n++)
    {
      const char *option = options[n];
      throw_error (context, ERROR_INVALID_OPTION, "Unknown option %s", option);
      goto out;
    }

  inhibitor = inhibitor_new (context);

  device->priv->polling_inhibitors = g_list_prepend (device->priv->polling_inhibitors, inhibitor);
  g_signal_connect (inhibitor, "disconnected", G_CALLBACK (polling_inhibitor_disconnected_cb), device);

  update_info (device);
  drain_pending_changes (device, FALSE);
  daemon_local_update_poller (device->priv->daemon);

  dbus_g_method_return (context, inhibitor_get_cookie (inhibitor));

 out:
  ;
}
