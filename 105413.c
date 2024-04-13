device_drive_uninhibit_polling (Device *device,
                                char *cookie,
                                DBusGMethodInvocation *context)
{
  const gchar *sender;
  Inhibitor *inhibitor;
  GList *l;

  sender = dbus_g_method_get_sender (context);

  inhibitor = NULL;
  for (l = device->priv->polling_inhibitors; l != NULL; l = l->next)
    {
      Inhibitor *i = INHIBITOR (l->data);

      if (g_strcmp0 (inhibitor_get_unique_dbus_name (i), sender) == 0 && g_strcmp0 (inhibitor_get_cookie (i), cookie)
          == 0)
        {
          inhibitor = i;
          break;
        }
    }

  if (inhibitor == NULL)
    {
      throw_error (context, ERROR_FAILED, "No such inhibitor");
      goto out;
    }

  device->priv->polling_inhibitors = g_list_remove (device->priv->polling_inhibitors, inhibitor);
  g_object_unref (inhibitor);

  update_info (device);
  drain_pending_changes (device, FALSE);
  daemon_local_update_poller (device->priv->daemon);

  dbus_g_method_return (context);

 out:
  return TRUE;
}
