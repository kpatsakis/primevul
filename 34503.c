get_start_time_for_pid (pid_t    pid,
                        GError **error)
{
  guint64 start_time;
#if !defined(HAVE_FREEBSD) && !defined(HAVE_NETBSD) && !defined(HAVE_OPENBSD)
  gchar *filename;
  gchar *contents;
  size_t length;
  gchar **tokens;
  guint num_tokens;
  gchar *p;
  gchar *endp;

  start_time = 0;
  contents = NULL;

  filename = g_strdup_printf ("/proc/%d/stat", pid);

  if (!g_file_get_contents (filename, &contents, &length, error))
    goto out;

  /* start time is the token at index 19 after the '(process name)' entry - since only this
   * field can contain the ')' character, search backwards for this to avoid malicious
   * processes trying to fool us
   */
  p = strrchr (contents, ')');
  if (p == NULL)
    {
      g_set_error (error,
                   POLKIT_ERROR,
                   POLKIT_ERROR_FAILED,
                   "Error parsing file %s",
                   filename);
      goto out;
    }
  p += 2; /* skip ') ' */
  if (p - contents >= (int) length)
    {
      g_set_error (error,
                   POLKIT_ERROR,
                   POLKIT_ERROR_FAILED,
                   "Error parsing file %s",
                   filename);
      goto out;
    }

  tokens = g_strsplit (p, " ", 0);

  num_tokens = g_strv_length (tokens);

  if (num_tokens < 20)
    {
      g_set_error (error,
                   POLKIT_ERROR,
                   POLKIT_ERROR_FAILED,
                   "Error parsing file %s",
                   filename);
      goto out;
    }

  start_time = strtoull (tokens[19], &endp, 10);
  if (endp == tokens[19])
    {
      g_set_error (error,
                   POLKIT_ERROR,
                   POLKIT_ERROR_FAILED,
                   "Error parsing file %s",
                   filename);
      goto out;
    }

  g_strfreev (tokens);

 out:
  g_free (filename);
  g_free (contents);
#else
#ifdef HAVE_NETBSD
  struct kinfo_proc2 p;
#else
  struct kinfo_proc p;
#endif

  start_time = 0;

  if (! get_kinfo_proc (pid, &p))
    {
      g_set_error (error,
                   POLKIT_ERROR,
                   POLKIT_ERROR_FAILED,
                   "Error obtaining start time for %d (%s)",
                   (gint) pid,
                   g_strerror (errno));
      goto out;
    }

#ifdef HAVE_FREEBSD
  start_time = (guint64) p.ki_start.tv_sec;
#else
  start_time = (guint64) p.p_ustart_sec;
#endif

out:
#endif

  return start_time;
}
