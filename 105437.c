device_linux_md_check_authorized_cb (Daemon *daemon,
                                     Device *device,
                                     DBusGMethodInvocation *context,
                                     const gchar *action_id,
                                     guint num_user_data,
                                     gpointer *user_data_elements)
{
  gchar **options = user_data_elements[0];
  gchar *filename;
  int n, m;
  char *argv[128];
  const gchar *job_name;

  filename = NULL;

  if (!device->priv->device_is_linux_md)
    {
      throw_error (context, ERROR_FAILED, "Device is not a Linux md drive");
      goto out;
    }

  if (g_strcmp0 (device->priv->linux_md_sync_action, "idle") != 0)
    {
      throw_error (context, ERROR_FAILED, "Array is not idle");
      goto out;
    }

  n = 0;
  argv[n++] = "udisks-helper-linux-md-check";
  argv[n++] = device->priv->device_file;
  argv[n++] = device->priv->native_path;
  for (m = 0; options[m] != NULL; m++)
    {
      if (n >= (int) sizeof(argv) - 1)
        {
          throw_error (context, ERROR_FAILED, "Too many options");
          goto out;
        }
      /* the helper will validate each option */
      argv[n++] = (char *) options[m];
    }
  argv[n++] = NULL;

  job_name = "LinuxMdCheck";
  for (n = 0; options != NULL && options[n] != NULL; n++)
    if (strcmp (options[n], "repair") == 0)
      job_name = "LinuxMdRepair";

  if (!job_new (context, job_name, TRUE, device, argv, NULL, linux_md_check_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
