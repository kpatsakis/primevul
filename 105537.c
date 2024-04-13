linux_md_expand_completed_cb (DBusGMethodInvocation *context,
                                     Device *device,
                                     gboolean job_was_cancelled,
                                     int status,
                                     const char *stderr,
                                     const char *stdout,
                                     gpointer user_data)
{
  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      /* the kernel side of md currently doesn't emit a 'changed' event so
       * generate one since state may have changed (e.g. rebuild started etc.)
       */
      device_generate_kernel_change_event (device);

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
                       "Error expanding array: helper script exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
