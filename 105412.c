device_drive_set_spindown_timeout_authorized_cb (Daemon *daemon,
                                                 Device *device,
                                                 DBusGMethodInvocation *context,
                                                 const gchar *action_id,
                                                 guint num_user_data,
                                                 gpointer *user_data_elements)
{
  gint timeout_seconds = GPOINTER_TO_INT (user_data_elements[0]);
  gchar **options = user_data_elements[1];
  Inhibitor *inhibitor;
  guint n;

  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (!device->priv->drive_can_spindown)
    {
      throw_error (context, ERROR_FAILED, "Cannot spindown device");
      goto out;
    }

  if (timeout_seconds < 1)
    {
      throw_error (context, ERROR_FAILED, "Timeout seconds must be at least 1");
      goto out;
    }

  for (n = 0; options[n] != NULL; n++)
    {
      const char *option = options[n];
      throw_error (context, ERROR_INVALID_OPTION, "Unknown option %s", option);
      goto out;
    }

  inhibitor = inhibitor_new (context);

  g_object_set_data (G_OBJECT (inhibitor), "spindown-timeout-seconds", GINT_TO_POINTER (timeout_seconds));

  device->priv->spindown_inhibitors = g_list_prepend (device->priv->spindown_inhibitors, inhibitor);
  g_signal_connect (inhibitor, "disconnected", G_CALLBACK (spindown_inhibitor_disconnected_cb), device);

  update_info (device);
  drain_pending_changes (device, FALSE);
  daemon_local_update_spindown (device->priv->daemon);

  dbus_g_method_return (context, inhibitor_get_cookie (inhibitor));

 out:
  ;
}
