linux_md_check_completed_cb (DBusGMethodInvocation *context,
                             Device *device,
                             gboolean job_was_cancelled,
                             int status,
                             const char *stderr,
                             const char *stdout,
                             gpointer user_data)
{
  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      guint64 num_errors;

      num_errors = sysfs_get_uint64 (device->priv->native_path, "md/mismatch_cnt");

      dbus_g_method_return (context, num_errors);
    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error checking array: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
