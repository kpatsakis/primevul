linux_lvm2_lv_create_completed_cb (DBusGMethodInvocation *context,
                                   Device *device,
                                   gboolean job_was_cancelled,
                                   int status,
                                   const char *stderr,
                                   const char *stdout,
                                   gpointer user_data)
{
  CreateLvm2LVData *data = user_data;

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      Device *d;

      d = lvm2_lv_create_has_lv (data);
      if (d != NULL)
        {
          /* yay! it is.. now create the file system if requested */
          lvm2_lv_create_found_device (device, data);
        }
      else
        {
          /* otherwise sit around and wait for the new LV to appear */
          data->device_added_signal_handler_id = g_signal_connect_after (data->daemon,
                                                                         "device-added",
                                                                         G_CALLBACK (lvm2_lv_create_device_added_cb),
                                                                         data);
          data->device_changed_signal_handler_id = g_signal_connect_after (data->daemon,
                                                                           "device-changed",
                                                                         G_CALLBACK (lvm2_lv_create_device_changed_cb),
                                                                           data);
          data->device_added_timeout_id = g_timeout_add (10 * 1000,
                                                         lvm2_lv_create_device_not_seen_cb,
                                                         data);

          lvm2_lv_create_data_ref (data);
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
                       "Error creating LVM2 Logical Volume: lvcreate exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
