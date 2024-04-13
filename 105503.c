get_command_line_for_pid (pid_t pid)
{
  char proc_name[32];
  char *buf;
  gsize len;
  char *ret;
  unsigned int n;

  ret = NULL;

  snprintf (proc_name, sizeof(proc_name), "/proc/%d/cmdline", pid);
  if (g_file_get_contents (proc_name, &buf, &len, NULL))
    {
      for (n = 0; n < len; n++)
        {
          if (buf[n] == '\0')
            buf[n] = ' ';
        }
      g_strstrip (buf);
      ret = buf;
    }

  return ret;
}
