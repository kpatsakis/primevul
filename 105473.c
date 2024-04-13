drive_eject_completed_cb (DBusGMethodInvocation *context,
                          Device *device,
                          gboolean job_was_cancelled,
                          int status,
                          const char *stderr,
                          const char *stdout,
                          gpointer user_data)
{
  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      /* TODO: probably wait for has_media to change to FALSE */
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
          throw_error (context,
                       ERROR_FAILED,
                       "Error ejecting: eject exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
