include(WebKitWebView *page, GArray *argv, GString *result) {
    (void) page;
    (void) result;
    gchar *pe   = NULL,
          *path = NULL,
          *line;
    int i=0;

    if(!argv_idx(argv, 0))
        return;

    pe = parseenv(argv_idx(argv, 0));
    if((path = find_existing_file(pe))) {
        GArray* lines = read_file_by_line(path);

        while ((line = g_array_index(lines, gchar*, i))) {
            parse_cmd_line (line, NULL);
            i++;
            g_free (line);
        }
        g_array_free (lines, TRUE);

        send_event(FILE_INCLUDED, path, NULL);
        g_free(path);
    }
    g_free(pe);
}
