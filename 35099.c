assert_sync_beginswith_stdarg() {
    GString *stdargs = g_string_new("");

    g_string_append_printf(stdargs, "%s %d %d ", uzbl.state.config_file, getpid(), (int)uzbl.xwin);
    g_string_append_printf(stdargs, "%s %s ", uzbl.comm.fifo_path, uzbl.comm.socket_path);
    g_string_append_printf(stdargs, "%s %s ", uzbl.state.uri, uzbl.gui.main_title);

    gchar *rest = assert_str_beginswith(stdargs, uzbl.comm.sync_stdout);

    g_string_free(stdargs, TRUE);

    return rest;
}
