linux_md_create_completed_cb (DBusGMethodInvocation *context,
                              Device *device,
                              gboolean job_was_cancelled,
                              int status,
                              const char *stderr,
                              const char *stdout,
                              gpointer user_data)
{
  LinuxMdCreateData *data = user_data;

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      GList *l;
      GList *devices;
      char *objpath;

      /* see if the component appeared already */

      objpath = NULL;

      devices = daemon_local_get_all_devices (data->daemon);
      for (l = devices; l != NULL; l = l->next)
        {
          Device *device = DEVICE (l->data);

          if (device->priv->device_is_linux_md)
            {

              /* TODO: check properly */

              /* yup, return to caller */
              objpath = device->priv->object_path;
              break;
            }
        }

      g_list_free (devices);

      if (objpath != NULL)
        {
          dbus_g_method_return (context, objpath);
        }
      else
        {
          /* sit around and wait for the md array to appear */

          /* sit around wait for the cleartext device to appear */
          data->device_added_signal_handler_id = g_signal_connect_after (data->daemon,
                                                                         "device-added",
                                                                         (GCallback) linux_md_create_device_added_cb,
                                                                         linux_md_create_data_ref (data));

          /* set up timeout for error reporting if waiting failed
           *
           * (the signal handler and the timeout handler share the ref to data
           * as one will cancel the other)
           */
          data->device_added_timeout_id = g_timeout_add (10 * 1000, linux_md_create_device_not_seen_cb, data);
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
                       "Error assembling array: mdadm exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
