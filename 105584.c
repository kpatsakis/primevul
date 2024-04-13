partition_table_create_completed_cb (DBusGMethodInvocation *context,
                                     Device *device,
                                     gboolean job_was_cancelled,
                                     int status,
                                     const char *stderr,
                                     const char *stdout,
                                     gpointer user_data)
{
  CreatePartitionTableData *data = user_data;

  /* poke the kernel so we can reread the data */
  device_generate_kernel_change_event (device);

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

      if (g_strcmp0 (device->priv->partition_table_scheme, data->scheme) == 0)
        {
          dbus_g_method_return (context);
        }
      else
        {
          /* sit around and wait for the new partition table to appear */
          data->device_changed_signal_handler_id
            = g_signal_connect_after (device->priv->daemon,
                                      "device-changed",
                                      (GCallback) partition_table_create_device_changed_cb,
                                      partition_table_create_data_ref (data));

          /* set up timeout for error reporting if waiting failed
           *
           * (the signal handler and the timeout handler share the ref to data
           * as one will cancel the other)
           */
          data->device_changed_timeout_id = g_timeout_add (10 * 1000,
                                                           partition_table_create_device_not_changed_cb,
                                                           data);
        }

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
                       "Error creating partition table: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
