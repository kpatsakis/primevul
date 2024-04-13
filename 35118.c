chain (WebKitWebView *page, GArray *argv, GString *result) {
    (void) page; (void) result;
    gchar *a = NULL;
    gchar **parts = NULL;
    guint i = 0;
    while ((a = argv_idx(argv, i++))) {
        parts = g_strsplit (a, " ", 2);
        if (parts[0])
          parse_command(parts[0], parts[1], result);
        g_strfreev (parts);
    }
}
