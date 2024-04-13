job_child_watch_cb (GPid pid,
                    int status,
                    gpointer user_data)
{
  char *buf;
  gsize buf_size;
  Job *job = user_data;

  if (g_io_channel_read_to_end (job->error_channel, &buf, &buf_size, NULL) == G_IO_STATUS_NORMAL)
    {
      g_string_append_len (job->error_string, buf, buf_size);
      g_free (buf);
    }
  if (g_io_channel_read_to_end (job->out_channel, &buf, &buf_size, NULL) == G_IO_STATUS_NORMAL)
    {
      g_string_append_len (job->stdout_string, buf, buf_size);
      g_free (buf);
    }

  PROFILE ("job finish (id=%s, pid=%i, device=%s)", job->job_id, job->pid, job->device ? job->device->priv->device_file : "none");
  g_print ("helper(pid %5d): completed with exit code %d\n", job->pid, WEXITSTATUS (status));

  job->status = status;

  /* if requested, run 'udevadm settle' on success */
  if (!job->was_cancelled && WIFEXITED (status) && WEXITSTATUS (status) == 0 && job->udev_settle)
    {
      GError *error;
      gchar *argv[] =
        { "udevadm", "settle", "--quiet", NULL };
      GPid udevadm_pid;

      error = NULL;
      if (!g_spawn_async (NULL,
                          argv,
                          NULL,
                          G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
                          NULL,
                          NULL,
                          &udevadm_pid,
                          &error))
        {
          g_warning ("Error running 'udevadm settle --quiet': %s", error->message);
          g_error_free (error);

          job_complete (job);
        }
      else
        {
          g_child_watch_add (udevadm_pid, job_udevadm_settle_child_cb, job);
        }
    }
  else
    {
      /* return immediately on error */
      job_complete (job);
    }
}
