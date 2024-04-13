partition_modify_completed_cb (DBusGMethodInvocation *context,
                               Device *device,
                               gboolean job_was_cancelled,
                               int status,
                               const char *stderr,
                               const char *stdout,
                               gpointer user_data)
{
  ModifyPartitionData *data = user_data;

  /* poke the kernel so we can reread the data */
  device_generate_kernel_change_event (data->enclosing_device);
  device_generate_kernel_change_event (data->device);

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      /* update local copy, don't wait for the kernel */

      device_set_partition_type (device, data->type);
      device_set_partition_label (device, data->label);
      device_set_partition_flags (device, data->flags);

      drain_pending_changes (device, FALSE);

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
                       "Error modifying partition: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
