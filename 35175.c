run_command (const gchar *command, const guint npre, const gchar **args,
             const gboolean sync, char **output_stdout) {
    GError *err = NULL;

    GArray *a = g_array_new (TRUE, FALSE, sizeof(gchar*));
    gchar *pid = itos(getpid());
    gchar *xwin = itos(uzbl.xwin);
    guint i;
    sharg_append(a, command);
    for (i = 0; i < npre; i++) /* add n args before the default vars */
        sharg_append(a, args[i]);
    sharg_append(a, uzbl.state.config_file);
    sharg_append(a, pid);
    sharg_append(a, xwin);
    sharg_append(a, uzbl.comm.fifo_path);
    sharg_append(a, uzbl.comm.socket_path);
    sharg_append(a, uzbl.state.uri);
    sharg_append(a, uzbl.gui.main_title);

    for (i = npre; i < g_strv_length((gchar**)args); i++)
        sharg_append(a, args[i]);

    gboolean result;
    if (sync) {
        if (*output_stdout) *output_stdout = strfree(*output_stdout);

        result = g_spawn_sync(NULL, (gchar **)a->data, NULL, G_SPAWN_SEARCH_PATH,
                              NULL, NULL, output_stdout, NULL, NULL, &err);
    } else
        result = g_spawn_async(NULL, (gchar **)a->data, NULL, G_SPAWN_SEARCH_PATH,
                                  NULL, NULL, NULL, &err);

    if (uzbl.state.verbose) {
        GString *s = g_string_new("spawned:");
        for (i = 0; i < (a->len); i++) {
            gchar *qarg = g_shell_quote(g_array_index(a, gchar*, i));
            g_string_append_printf(s, " %s", qarg);
            g_free (qarg);
        }
        g_string_append_printf(s, " -- result: %s", (result ? "true" : "false"));
        printf("%s\n", s->str);
        g_string_free(s, TRUE);
        if(output_stdout) {
            printf("Stdout: %s\n", *output_stdout);
        }
    }
    if (err) {
        g_printerr("error on run_command: %s\n", err->message);
        g_error_free (err);
    }
    g_free (pid);
    g_free (xwin);
    g_array_free (a, TRUE);
    return result;
}
