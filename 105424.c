device_filesystem_set_label_authorized_cb (Daemon *daemon,
                                           Device *device,
                                           DBusGMethodInvocation *context,
                                           const gchar *action_id,
                                           guint num_user_data,
                                           gpointer *user_data_elements)
{
  const gchar *new_label = user_data_elements[0];
  int n;
  char *argv[10];
  const Filesystem *fs_details;
  GError *error;

  error = NULL;

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "filesystem") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a mountable file system");
      goto out;
    }

  fs_details = daemon_local_get_fs_details (device->priv->daemon, device->priv->id_type);
  if (fs_details == NULL)
    {
      throw_error (context, ERROR_BUSY, "Unknown filesystem");
      goto out;
    }

  if (!fs_details->supports_online_label_rename)
    {
      if (device_local_is_busy (device, FALSE, &error))
        {
          dbus_g_method_return_error (context, error);
          g_error_free (error);
          goto out;
        }
    }

  n = 0;
  argv[n++] = "udisks-helper-change-filesystem-label";
  argv[n++] = device->priv->device_file;
  argv[n++] = device->priv->id_type;
  argv[n++] = (char *) new_label;
  argv[n++] = NULL;

  if (!job_new (context,
                "FilesystemSetLabel",
                FALSE,
                device,
                argv,
                NULL,
                filesystem_set_label_completed_cb,
                FALSE,
                g_strdup (new_label),
                g_free))
    {
      goto out;
    }

 out:
  ;
}
