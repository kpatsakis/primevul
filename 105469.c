drive_ata_smart_initiate_selftest_completed_cb (DBusGMethodInvocation *context,
                                                Device *device,
                                                gboolean job_was_cancelled,
                                                int status,
                                                const char *stderr,
                                                const char *stdout,
                                                gpointer user_data)
{
  char *options[] =
    { NULL };

  /* no matter what happened, refresh the data */
  device_drive_ata_smart_refresh_data (device, options, NULL);

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
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error running self test: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
