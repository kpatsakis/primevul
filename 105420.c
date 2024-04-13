device_filesystem_list_open_files_authorized_cb (Daemon *daemon,
                                                 Device *device,
                                                 DBusGMethodInvocation *context,
                                                 const gchar *action_id,
                                                 guint num_user_data,
                                                 gpointer *user_data_elements)
{
  int n;
  char *argv[16];
  GError *error;

  if (!device->priv->device_is_mounted || device->priv->device_mount_paths->len == 0)
    {
      throw_error (context, ERROR_FAILED, "Device is not mounted");
      goto out;
    }

  n = 0;
  argv[n++] = "lsof";
  argv[n++] = "-t";
  argv[n++] = ((gchar **) device->priv->device_mount_paths->pdata)[0];
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context, NULL, /* don't run this as a job */
                FALSE, device, argv, NULL, filesystem_list_open_files_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
