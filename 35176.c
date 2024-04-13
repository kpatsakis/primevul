run_external_js (WebKitWebView * web_view, GArray *argv, GString *result) {
    (void) result;
    gchar *path = NULL;

    if (argv_idx(argv, 0) &&
        ((path = find_existing_file(argv_idx(argv, 0)))) ) {
        GArray* lines = read_file_by_line (path);
        gchar*  js = NULL;
        int i = 0;
        gchar* line;

        while ((line = g_array_index(lines, gchar*, i))) {
            if (js == NULL) {
                js = g_strdup (line);
            } else {
                gchar* newjs = g_strconcat (js, line, NULL);
                js = newjs;
            }
            i ++;
            g_free (line);
        }

        if (uzbl.state.verbose)
            printf ("External JavaScript file %s loaded\n", argv_idx(argv, 0));

        gchar* newjs = str_replace("%s", argv_idx (argv, 1)?argv_idx (argv, 1):"", js);
        g_free (js);
        js = newjs;

        eval_js (web_view, js, result);
        g_free (js);
        g_array_free (lines, TRUE);
        g_free(path);
    }
}
