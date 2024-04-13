linux_md_remove_component_completed_cb (DBusGMethodInvocation *context,
                                        Device *device,
                                        gboolean job_was_cancelled,
                                        int status,
                                        const char *stderr,
                                        const char *stdout,
                                        gpointer user_data)
{
  RemoveComponentData *data = user_data;

  /* the slave got new metadata on it; reread that */
  device_generate_kernel_change_event (data->slave);

  /* the kernel side of md currently doesn't emit a 'changed' event so
   * generate one since state may have changed (e.g. rebuild started etc.)
   */
  device_generate_kernel_change_event (device);

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

      /* wait for the slave to be busy, then start erasing it */

      data->device_changed_signal_handler_id
        = g_signal_connect_after (device->priv->daemon,
                                  "device-changed",
                                  (GCallback) linux_md_remove_component_device_changed_cb,
                                  remove_component_data_ref (data));

      /* set up timeout for error reporting if waiting failed
       *
       * (the signal handler and the timeout handler share the ref to data
       * as one will cancel the other)
       */
      data->device_changed_timeout_id = g_timeout_add (10 * 1000, linux_md_remove_component_device_not_seen_cb, data);

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
                       "Error removing component: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
