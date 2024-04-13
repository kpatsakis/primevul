_polkit_subject_get_cmdline (PolkitSubject *subject)
{
  PolkitSubject *process;
  gchar *ret;
  gint pid;
  gchar *filename;
  gchar *contents;
  gsize contents_len;
  GError *error;
  guint n;

  g_return_val_if_fail (subject != NULL, NULL);

  error = NULL;

  ret = NULL;
  process = NULL;
  filename = NULL;
  contents = NULL;

  if (POLKIT_IS_UNIX_PROCESS (subject))
    {
      process = g_object_ref (subject);
    }
  else if (POLKIT_IS_SYSTEM_BUS_NAME (subject))
    {
      process = polkit_system_bus_name_get_process_sync (POLKIT_SYSTEM_BUS_NAME (subject),
                                                         NULL,
                                                         &error);
      if (process == NULL)
        {
          g_printerr ("Error getting process for system bus name `%s': %s\n",
                      polkit_system_bus_name_get_name (POLKIT_SYSTEM_BUS_NAME (subject)),
                      error->message);
          g_error_free (error);
          goto out;
        }
    }
  else
    {
      g_warning ("Unknown subject type passed to _polkit_subject_get_cmdline()");
      goto out;
    }

  pid = polkit_unix_process_get_pid (POLKIT_UNIX_PROCESS (process));

  filename = g_strdup_printf ("/proc/%d/cmdline", pid);

  if (!g_file_get_contents (filename,
                            &contents,
                            &contents_len,
                            &error))
    {
      g_printerr ("Error opening `%s': %s\n",
                  filename,
                  error->message);
      g_error_free (error);
      goto out;
    }

  if (contents == NULL || contents_len == 0)
    {
      goto out;
    }
  else
    {
      /* The kernel uses '\0' to separate arguments - replace those with a space. */
      for (n = 0; n < contents_len - 1; n++)
        {
          if (contents[n] == '\0')
            contents[n] = ' ';
        }
      ret = g_strdup (contents);
      g_strstrip (ret);
    }

 out:
  g_free (filename);
  g_free (contents);
  if (process != NULL)
    g_object_unref (process);
  return ret;
}
