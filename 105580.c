partition_delete_completed_cb (DBusGMethodInvocation *context,
                               Device *device,
                               gboolean job_was_cancelled,
                               int status,
                               const char *stderr,
                               const char *stdout,
                               gpointer user_data)
{
  Device *enclosing_device = DEVICE (user_data);

  /* poke the kernel about the enclosing disk so we can reread the partitioning table */
  device_generate_kernel_change_event (enclosing_device);

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
                       "Error erasing: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
