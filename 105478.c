filesystem_check_completed_cb (DBusGMethodInvocation *context,
                               Device *device,
                               gboolean job_was_cancelled,
                               int status,
                               const char *stderr,
                               const char *stdout,
                               gpointer user_data)
{
  if (WIFEXITED (status) && !job_was_cancelled)
    {
      int rc;
      gboolean fs_is_clean;

      fs_is_clean = FALSE;

      rc = WEXITSTATUS (status);
      if ((rc == 0) || (((rc & 1) != 0) && ((rc & 4) == 0)))
        {
          fs_is_clean = TRUE;
        }

      dbus_g_method_return (context, fs_is_clean);
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
                       "Error fsck'ing: fsck exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
