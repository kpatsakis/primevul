job_new (DBusGMethodInvocation *context,
         const char *job_id,
         gboolean is_cancellable,
         Device *device,
         char **argv,
         const char *stdin_str,
         JobCompletedFunc job_completed_func,
         gboolean udev_settle, /* if TRUE, runs udevsettle before returning if the command succeeded */
         gpointer user_data,
         GDestroyNotify user_data_destroy_func)
{
  Job *job;
  gboolean ret;
  GError *error;

  ret = FALSE;
  job = NULL;

  PROFILE ("job_new(id=%s, device=%s): start", job_id ? job_id : argv[0], device ? device->priv->device_file : "none");

  if (device != NULL)
    {
      if (device->priv->job != NULL || device->priv->job_in_progress)
        {
          throw_error (context, ERROR_BUSY, "There is already a job running");
          goto out;
        }
    }

  job = g_new0 (Job, 1);
  job->context = context;
  job->device = device != NULL ? DEVICE (g_object_ref (device)) : NULL;
  job->job_completed_func = job_completed_func;
  job->user_data = user_data;
  job->user_data_destroy_func = user_data_destroy_func;
  job->stderr_fd = -1;
  job->stdout_fd = -1;
  job->stdin_fd = -1;
  job->stdin_str = g_strdup (stdin_str);
  job->stdin_cursor = job->stdin_str;
  job->stdout_string = g_string_sized_new (1024);
  job->job_id = g_strdup (job_id);
  job->udev_settle = udev_settle;

  if (device != NULL && job_id != NULL)
    {
      g_free (job->device->priv->job_id);
      job->device->priv->job_id = g_strdup (job_id);
    }

  error = NULL;
  if (!g_spawn_async_with_pipes (NULL,
                                 argv,
                                 NULL,
                                 G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
                                 NULL,
                                 NULL,
                                 &(job->pid),
                                 stdin_str != NULL ? &(job->stdin_fd) : NULL,
                                 &(job->stdout_fd),
                                 &(job->stderr_fd),
                                 &error))
    {
      throw_error (context, ERROR_FAILED, "Error starting job: %s", error->message);
      g_error_free (error);
      goto out;
    }

  g_child_watch_add (job->pid, job_child_watch_cb, job);

  job->error_string = g_string_new ("");
  job->error_channel = g_io_channel_unix_new (job->stderr_fd);
  error = NULL;
  if (g_io_channel_set_flags (job->error_channel, G_IO_FLAG_NONBLOCK, &error) != G_IO_STATUS_NORMAL)
    {
      g_warning ("Cannon set stderr fd for child to be non blocking: %s", error->message);
      g_error_free (error);
    }
  job->error_channel_source_id = g_io_add_watch (job->error_channel, G_IO_IN, job_read_error, job);

  job->out_channel = g_io_channel_unix_new (job->stdout_fd);
  error = NULL;
  if (g_io_channel_set_flags (job->out_channel, G_IO_FLAG_NONBLOCK, &error) != G_IO_STATUS_NORMAL)
    {
      g_warning ("Cannon set stdout fd for child to be non blocking: %s", error->message);
      g_error_free (error);
    }
  job->out_channel_source_id = g_io_add_watch (job->out_channel, G_IO_IN, job_read_out, job);

  if (job->stdin_fd >= 0)
    {
      job->in_channel = g_io_channel_unix_new (job->stdin_fd);
      if (g_io_channel_set_flags (job->in_channel, G_IO_FLAG_NONBLOCK, &error) != G_IO_STATUS_NORMAL)
        {
          g_warning ("Cannon set stdin fd for child to be non blocking: %s", error->message);
          g_error_free (error);
        }
      job->in_channel_source_id = g_io_add_watch (job->in_channel, G_IO_OUT, job_write_in, job);
    }

  ret = TRUE;

  if (device != NULL && job_id != NULL)
    {
      device->priv->job_in_progress = TRUE;
      device->priv->job_is_cancellable = is_cancellable;
      device->priv->job_percentage = -1.0;
      device->priv->job_initiated_by_uid = 0;
      if (context != NULL)
        {
          daemon_local_get_uid (device->priv->daemon, &(device->priv->job_initiated_by_uid), context);
        }

      device->priv->job = job;

      emit_job_changed (device);
    }

  if (device != NULL)
    {
      g_print ("helper(pid %5d): launched job %s on %s\n", job->pid, argv[0], device->priv->device_file);
    }
  else
    {
      g_print ("helper(pid %5d): launched job %s on daemon\n", job->pid, argv[0]);
    }

 out:
  if (!ret && job != NULL)
    job_free (job);
  PROFILE ("job_new(id=%s, device=%s): end", job_id, device ? device->priv->device_file : "none");
  return ret;
}
