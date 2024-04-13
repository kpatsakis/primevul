luks_change_passphrase_completed_cb (DBusGMethodInvocation *context,
                                     Device *device,
                                     gboolean job_was_cancelled,
                                     int status,
                                     const char *stderr,
                                     const char *stdout,
                                     gpointer user_data)
{
  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      dbus_g_method_return (context);
    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else if (WEXITSTATUS (status) == 3)
        {
          throw_error (context,
                       ERROR_FILESYSTEM_TOOLS_MISSING,
                       "Error changing fs label: tool not available: %s",
                       stderr);
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error changing secret on device: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
