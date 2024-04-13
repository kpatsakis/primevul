device_filesystem_mount (Device *device,
                         const char *filesystem_type,
                         char **given_options,
                         DBusGMethodInvocation *context)
{
  const gchar *action_id;
  gboolean auth_no_user_interaction;
  gchar **options_to_pass;
  guint n;
  guint m;

  if (is_device_in_fstab (device, NULL))
    {
      action_id = NULL;
    }
  else
    {
      if (device->priv->device_is_system_internal)
        action_id = "org.freedesktop.udisks.filesystem-mount-system-internal";
      else
        action_id = "org.freedesktop.udisks.filesystem-mount";
    }

  auth_no_user_interaction = FALSE;
  options_to_pass = g_strdupv (given_options);
  for (n = 0; options_to_pass != NULL && options_to_pass[n] != NULL; n++)
    {
      if (g_strcmp0 (options_to_pass[n], "auth_no_user_interaction") == 0)
        {
          auth_no_user_interaction = TRUE;
          g_free (options_to_pass[n]);
          for (m = n; options_to_pass[m + 1] != NULL; m++)
            options_to_pass[m] = options_to_pass[m + 1];
          options_to_pass[m] = NULL;
          break;
        }
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           action_id,
                           "FilesystemMount",
                           !auth_no_user_interaction,
                           device_filesystem_mount_authorized_cb,
                           context,
                           2,
                           g_strdup (filesystem_type),
                           g_free,
                           options_to_pass,
                           g_strfreev);

  return TRUE;
}
