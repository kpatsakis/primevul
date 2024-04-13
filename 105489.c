filesystem_unmount_completed_cb (DBusGMethodInvocation *context,
                                 Device *device,
                                 gboolean job_was_cancelled,
                                 int status,
                                 const char *stderr,
                                 const char *stdout,
                                 gpointer user_data)
{
  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      /* update_info_mount_state() will update the mounts file and clean up the directory if needed */
      update_info (device);
      dbus_g_method_return (context);
    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else
        {
          if (strstr (stderr, "device is busy") != NULL)
            {
              throw_error (context, ERROR_BUSY, "Cannot unmount because file system on device is busy");
            }
          else
            {
              throw_error (context,
                           ERROR_FAILED,
                           "Error unmounting: umount exited with exit code %d: %s",
                           WEXITSTATUS (status),
                           stderr);
            }
        }
    }
}
