spawn_sh_sync(WebKitWebView *web_view, GArray *argv, GString *result) {
    (void)web_view; (void)result;
    if (!uzbl.behave.shell_cmd) {
        g_printerr ("spawn_sh_sync: shell_cmd is not set!\n");
        return;
    }

    guint i;
    gchar *spacer = g_strdup("");
    g_array_insert_val(argv, 1, spacer);
    gchar **cmd = split_quoted(uzbl.behave.shell_cmd, TRUE);

    for (i = 1; i < g_strv_length(cmd); i++)
        g_array_prepend_val(argv, cmd[i]);

    if (cmd) run_command(cmd[0], g_strv_length(cmd) + 1, (const gchar **) argv->data,
                         TRUE, &uzbl.comm.sync_stdout);
    g_free (spacer);
    g_strfreev (cmd);
}
