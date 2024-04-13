device_drive_poll_media_authorized_cb (Daemon *daemon,
                                       Device *device,
                                       DBusGMethodInvocation *context,
                                       const gchar *action_id,
                                       guint num_user_data,
                                       gpointer *user_data_elements)
{
  int n;
  char *argv[16];

  n = 0;
  argv[n++] = "udisks-helper-drive-poll";
  argv[n++] = device->priv->device_file;
  argv[n++] = NULL;

  if (!job_new (context, "DrivePollMedia", FALSE, device, argv, NULL, drive_poll_media_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
