job_write_in (GIOChannel *channel,
              GIOCondition condition,
              gpointer user_data)
{
  Job *job = user_data;
  gsize bytes_written;

  if (job->stdin_cursor == NULL || job->stdin_cursor[0] == '\0')
    {
      /* nothing left to write; remove ourselves */
      return FALSE;
    }

  g_io_channel_write_chars (channel, job->stdin_cursor, strlen (job->stdin_cursor), &bytes_written, NULL);
  g_io_channel_flush (channel, NULL);
  job->stdin_cursor += bytes_written;
  return TRUE;
}
