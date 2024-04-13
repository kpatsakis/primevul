filesystem_mount_completed_cb (DBusGMethodInvocation *context,
                               Device *device,
                               gboolean job_was_cancelled,
                               int status,
                               const char *stderr,
                               const char *stdout,
                               gpointer user_data)
{
  MountData *data = (MountData *) user_data;
  uid_t uid;

  daemon_local_get_uid (device->priv->daemon, &uid, context);

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

      update_info (device);
      drain_pending_changes (device, FALSE);
      unlock_cd_tray (device);

      dbus_g_method_return (context, data->mount_point);
    }
  else
    {
      if (data->remove_dir_on_unmount)
        {
          mount_file_remove (device->priv->device_file, data->mount_point);
          if (g_rmdir (data->mount_point) != 0)
            {
              g_warning ("Error removing dir in late mount error path: %m");
            }
        }

      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else if (WEXITSTATUS (status) == 32)
        {
          throw_error (context, ERROR_FILESYSTEM_DRIVER_MISSING, "Error mounting: %s", stderr);
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error mounting: mount exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
