set_var(WebKitWebView *page, GArray *argv, GString *result) {
    (void) page; (void) result;

    if(!argv_idx(argv, 0))
        return;

    gchar **split = g_strsplit(argv_idx(argv, 0), "=", 2);
    if (split[0] != NULL) {
        gchar *value = parseenv(split[1] ? g_strchug(split[1]) : " ");
        set_var_value(g_strstrip(split[0]), value);
        g_free(value);
    }
    g_strfreev(split);
}
