job_free (Job *job)
{
  if (job->user_data_destroy_func != NULL)
    job->user_data_destroy_func (job->user_data);
  if (job->device != NULL)
    g_object_unref (job->device);
  if (job->stderr_fd >= 0)
    close (job->stderr_fd);
  if (job->stdout_fd >= 0)
    close (job->stdout_fd);
  if (job->stdin_fd >= 0)
    {
      close (job->stdin_fd);
      g_source_remove (job->in_channel_source_id);
      g_io_channel_unref (job->in_channel);
    }
  g_source_remove (job->error_channel_source_id);
  g_source_remove (job->out_channel_source_id);
  g_io_channel_unref (job->error_channel);
  g_io_channel_unref (job->out_channel);
  g_string_free (job->error_string, TRUE);
  /* scrub stdin (may contain secrets) */
  if (job->stdin_str != NULL)
    {
      memset (job->stdin_str, '\0', strlen (job->stdin_str));
    }
  g_string_free (job->stdout_string, TRUE);
  g_free (job->stdin_str);
  g_free (job->job_id);
  g_free (job);
}
