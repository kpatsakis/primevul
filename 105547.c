linux_md_stop_completed_cb (DBusGMethodInvocation *context,
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
       * generate one such that the md device can disappear from our
       * database
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
                       "Error stopping array: mdadm exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
