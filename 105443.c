device_linux_md_stop_authorized_cb (Daemon *daemon,
                                    Device *device,
                                    DBusGMethodInvocation *context,
                                    const gchar *action_id,
                                    guint num_user_data,
                                    gpointer *user_data_elements)
{
  /* TODO: use options */
  int n;
  char *argv[10];
  GError *error;

  n = 0;
  argv[n++] = "mdadm";
  argv[n++] = "--stop";
  argv[n++] = device->priv->device_file;
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context, "LinuxMdStop", TRUE, device, argv, NULL, linux_md_stop_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
