spawn(WebKitWebView *web_view, GArray *argv, GString *result) {
    (void)web_view; (void)result;
    gchar *path = NULL;

    if (argv_idx(argv, 0) &&
            ((path = find_existing_file(argv_idx(argv, 0)))) ) {
        run_command(path, 0,
                ((const gchar **) (argv->data + sizeof(gchar*))),
                FALSE, NULL);
        g_free(path);
    }
}
