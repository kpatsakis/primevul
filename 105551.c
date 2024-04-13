lsof_parse (const char *stdout,
            GPtrArray *processes)
{
  int n;
  char **tokens;

  tokens = g_strsplit (stdout, "\n", 0);
  for (n = 0; tokens[n] != NULL; n++)
    {
      pid_t pid;
      uid_t uid;
      char *command_line;
      GValue elem =
        { 0 };

      if (strlen (tokens[n]) == 0)
        continue;

      pid = strtol (tokens[n], NULL, 0);
      uid = get_uid_for_pid (pid);
      command_line = get_command_line_for_pid (pid);

      g_value_init (&elem, LSOF_DATA_STRUCT_TYPE);
      g_value_take_boxed (&elem, dbus_g_type_specialized_construct (LSOF_DATA_STRUCT_TYPE));
      dbus_g_type_struct_set (&elem, 0, pid, 1, uid, 2, command_line != NULL ? command_line : "", G_MAXUINT);
      g_ptr_array_add (processes, g_value_get_boxed (&elem));

      g_free (command_line);
    }
  g_strfreev (tokens);
}
