job_read_error (GIOChannel *channel,
                GIOCondition condition,
                gpointer user_data)
{
  char buf[1024];
  gsize bytes_read;
  Job *job = user_data;

  g_io_channel_read_chars (channel, buf, sizeof buf, &bytes_read, NULL);
  g_string_append_len (job->error_string, buf, bytes_read);
  return TRUE;
}
