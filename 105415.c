device_filesystem_check_authorized_cb (Daemon *daemon,
                                       Device *device,
                                       DBusGMethodInvocation *context,
                                       const gchar *action_id,
                                       guint num_user_data,
                                       gpointer *user_data_elements)
{
  /* TODO: use options! */
  int n;
  char *argv[16];
  GError *error;

  /* TODO: change when we have a file system that supports online fsck */
  if (device->priv->device_is_mounted)
    {
      throw_error (context, ERROR_BUSY, "Device is mounted and no online capability in fsck tool for file system");
      goto out;
    }

  n = 0;
  argv[n++] = "fsck";
  argv[n++] = "-a";
  argv[n++] = device->priv->device_file;
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context, "FilesystemCheck", FALSE, device, argv, NULL, filesystem_check_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
