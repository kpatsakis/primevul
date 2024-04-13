device_filesystem_unmount_authorized_cb (Daemon *daemon,
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
  gboolean force_unmount;
  char *mount_path;
  uid_t uid;
  gchar uid_buf[32];

  mount_path = NULL;

  if (!device->priv->device_is_mounted || device->priv->device_mount_paths->len == 0)
    {
      throw_error (context, ERROR_FAILED, "Device is not mounted");
      goto out;
    }

  force_unmount = FALSE;
  for (n = 0; options[n] != NULL; n++)
    {
      char *option = options[n];
      if (strcmp ("force", option) == 0)
        {
          force_unmount = TRUE;
        }
      else
        {
          throw_error (context, ERROR_INVALID_OPTION, "Unknown option %s", option);
          goto out;
        }
    }

  daemon_local_get_uid (device->priv->daemon, &uid, context);
  g_snprintf (uid_buf, sizeof uid_buf, "%d", uid);

  if (!mount_file_has_device (device->priv->device_file, NULL, NULL))
    {
      if (is_device_in_fstab (device, &mount_path))
        {

          n = 0;
          argv[n++] = "udisks-helper-fstab-mounter";
          if (force_unmount)
            argv[n++] = "force_unmount";
          else
            argv[n++] = "unmount";
          argv[n++] = device->priv->device_file;
          argv[n++] = uid_buf;
          argv[n++] = NULL;
          goto run_job;
        }

      /* otherwise the user will have the .unmount-others authorization per the logic in
       * device_filesystem_unmount()
       */
    }

  mount_path = g_strdup (((gchar **) device->priv->device_mount_paths->pdata)[0]);

  n = 0;
  argv[n++] = "umount";
  if (force_unmount)
    {
      /* on Linux we currently only have lazy unmount to emulate this */
      argv[n++] = "-l";
    }
  argv[n++] = mount_path;
  argv[n++] = NULL;

 run_job:
  error = NULL;
  if (!job_new (context,
                "FilesystemUnmount",
                FALSE,
                device,
                argv,
                NULL,
                filesystem_unmount_completed_cb,
                FALSE,
                g_strdup (mount_path),
                g_free))
    {
      goto out;
    }

 out:
  g_free (mount_path);
}
