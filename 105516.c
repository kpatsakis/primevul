job_read_out (GIOChannel *channel,
              GIOCondition condition,
              gpointer user_data)
{
  char *s;
  char *line;
  char buf[1024];
  gsize bytes_read;
  Job *job = user_data;

  g_io_channel_read_chars (channel, buf, sizeof buf, &bytes_read, NULL);
  g_string_append_len (job->stdout_string, buf, bytes_read);

  do
    {
      gsize line_len;

      s = strstr (job->stdout_string->str + job->stdout_string_cursor, "\n");
      if (s == NULL)
        break;

      line_len = s - (job->stdout_string->str + job->stdout_string_cursor);
      line = g_strndup (job->stdout_string->str + job->stdout_string_cursor, line_len);
      job->stdout_string_cursor += line_len + 1;


      if (strlen (line) < 256)
        {
          double cur_percentage;
          ;

          if (sscanf (line, "udisks-helper-progress: %lg", &cur_percentage) == 1)
            {
              if (job->device != NULL && job->job_id != NULL)
                {
                  job->device->priv->job_percentage = cur_percentage;
                  emit_job_changed (job->device);
                }
            }
        }

      g_free (line);
    }
  while (TRUE);

  return TRUE;
}
