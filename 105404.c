device_drive_detach_authorized_cb (Daemon *daemon,
                                   Device *device,
                                   DBusGMethodInvocation *context,
                                   const gchar *action_id,
                                   guint num_user_data,
                                   gpointer *user_data_elements)
{
  gchar **options = user_data_elements[0];
  int n;
  char *argv[16];
  GError *error;
  char *mount_path;

  error = NULL;
  mount_path = NULL;

  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (!device->priv->drive_can_detach)
    {
      throw_error (context, ERROR_FAILED, "Device is not detachable");
      goto out;
    }

  if (device_local_is_busy (device, TRUE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  for (n = 0; options[n] != NULL; n++)
    {
      const char *option = options[n];
      throw_error (context, ERROR_INVALID_OPTION, "Unknown option %s", option);
      goto out;
    }

  n = 0;
  argv[n++] = "udisks-helper-drive-detach";
  argv[n++] = device->priv->device_file;
  argv[n++] = NULL;

  if (!job_new (context, "DriveDetach", FALSE, device, argv, NULL, drive_detach_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
