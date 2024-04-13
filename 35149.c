inject_handler_args(const gchar *actname, const gchar *origargs, const gchar *newargs) {
    /*
      If actname is one that calls an external command, this function will inject
      newargs in front of the user-provided args in that command line.  They will
      come become after the body of the script (in sh) or after the name of
      the command to execute (in spawn).
      i.e. sh <body> <userargs> becomes sh <body> <ARGS> <userargs> and
      spawn <command> <userargs> becomes spawn <command> <ARGS> <userargs>.

      The return value consist of two strings: the action (sh, ...) and its args.

      If act is not one that calls an external command, then the given action merely
      gets duplicated.
    */
    GArray *rets = g_array_new(TRUE, FALSE, sizeof(gchar*));
    /* Arrr! Here be memory leaks */
    gchar *actdup = g_strdup(actname);
    g_array_append_val(rets, actdup);

    if ((g_strcmp0(actname, "spawn") == 0) ||
        (g_strcmp0(actname, "sh") == 0) ||
        (g_strcmp0(actname, "sync_spawn") == 0) ||
        (g_strcmp0(actname, "sync_sh") == 0) ||
        (g_strcmp0(actname, "talk_to_socket") == 0)) {
        guint i;
        GString *a = g_string_new("");
        gchar **spawnparts = split_quoted(origargs, FALSE);
        g_string_append_printf(a, "%s", spawnparts[0]); /* sh body or script name */
        if (newargs) g_string_append_printf(a, " %s", newargs); /* handler args */

        for (i = 1; i < g_strv_length(spawnparts); i++) /* user args */
            if (spawnparts[i]) g_string_append_printf(a, " %s", spawnparts[i]);

        g_array_append_val(rets, a->str);
        g_string_free(a, FALSE);
        g_strfreev(spawnparts);
    } else {
        gchar *origdup = g_strdup(origargs);
        g_array_append_val(rets, origdup);
    }
    return (gchar**)g_array_free(rets, FALSE);
}
