filesystem_list_open_files_completed_cb (DBusGMethodInvocation *context,
                                         Device *device,
                                         gboolean job_was_cancelled,
                                         int status,
                                         const char *stderr,
                                         const char *stdout,
                                         gpointer user_data)
{
  if ((WEXITSTATUS (status) == 0 || WEXITSTATUS (status) == 1) && !job_was_cancelled)
    {
      GPtrArray *processes;

      processes = g_ptr_array_new ();
      lsof_parse (stdout, processes);
      dbus_g_method_return (context, processes);
      g_ptr_array_foreach (processes, (GFunc) g_value_array_free, NULL);
      g_ptr_array_free (processes, TRUE);
    }
  else
    {
      throw_error (context,
                   ERROR_FAILED,
                   "Error listing open files: lsof exited with exit code %d: %s",
                   WEXITSTATUS (status),
                   stderr);
    }
}
