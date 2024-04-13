partition_create_completed_cb (DBusGMethodInvocation *context,
                               Device *device,
                               gboolean job_was_cancelled,
                               int status,
                               const char *stderr,
                               const char *stdout,
                               gpointer user_data)
{
  CreatePartitionData *data = user_data;

  /* poke the kernel so we can reread the data */
  device_generate_kernel_change_event (device);

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      int n;
      int m;
      guint64 offset;
      guint64 size;
      char **tokens;

      /* Find the
       *
       *   job-create-partition-offset:
       *   job-create-partition-size:
       *
       * lines and parse the new start and end. We need this
       * for waiting on the created partition (since the requested
       * start and size passed may not be honored due to disk/cylinder/sector
       * alignment reasons).
       */
      offset = 0;
      size = 0;
      m = 0;
      tokens = g_strsplit (stderr, "\n", 0);
      for (n = 0; tokens[n] != NULL; n++)
        {
          char *line = tokens[n];
          char *endp;

          if (m == 2)
            break;

          if (g_str_has_prefix (line, "job-create-partition-offset: "))
            {
              offset = strtoll (line + sizeof("job-create-partition-offset: ") - 1, &endp, 10);
              if (*endp == '\0')
                m++;
            }
          else if (g_str_has_prefix (line, "job-create-partition-size: "))
            {
              size = strtoll (line + sizeof("job-create-partition-size: ") - 1, &endp, 10);
              if (*endp == '\0')
                m++;
            }
        }
      g_strfreev (tokens);

      if (m != 2)
        {
          throw_error (context, ERROR_FAILED, "Error creating partition: internal error, expected to find new "
                       "start and end but m=%d", m);
        }
      else
        {
          gboolean found_device;
          GList *devices;
          GList *l;

          data->created_offset = offset;
          data->created_size = size;

          /* check if the device is already there */
          found_device = FALSE;
          devices = daemon_local_get_all_devices (device->priv->daemon);
          for (l = devices; l != NULL; l = l->next)
            {
              Device *d = DEVICE (l->data);

              if (d->priv->device_is_partition && strcmp (d->priv->partition_slave, data->device->priv->object_path)
                  == 0 && data->created_offset == d->priv->partition_offset && data->created_size
                  == d->priv->partition_size)
                {
                  /* yay! it is.. now create the file system if requested */
                  partition_create_found_device (d, data);
                  found_device = TRUE;
                  break;
                }
            }

          if (!found_device)
            {
              /* otherwise sit around and wait for the new partition to appear */
              data->device_added_signal_handler_id
                = g_signal_connect_after (device->priv->daemon,
                                          "device-added",
                                          (GCallback) partition_create_device_added_cb,
                                          partition_create_data_ref (data));

              /* set up timeout for error reporting if waiting failed
               *
               * (the signal handler and the timeout handler share the ref to data
               * as one will cancel the other)
               */
              data->device_added_timeout_id = g_timeout_add (10 * 1000, partition_create_device_not_seen_cb, data);
            }
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
                       "Error creating partition: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
