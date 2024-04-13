load_uri_imp(gchar *uri) {
    GString* newuri;
    if (g_strstr_len (uri, 11, "javascript:") != NULL) {
        eval_js(uzbl.gui.web_view, uri, NULL);
        return;
    }
    newuri = g_string_new (uri);
    if (!soup_uri_new(uri)) {
        GString* fullpath = g_string_new ("");
        if (g_path_is_absolute (newuri->str))
            g_string_assign (fullpath, newuri->str);
        else {
            gchar* wd;
            wd = g_get_current_dir ();
            g_string_assign (fullpath, g_build_filename (wd, newuri->str, NULL));
            free(wd);
        }
        struct stat stat_result;
        if (! g_stat(fullpath->str, &stat_result)) {
            g_string_prepend (fullpath, "file://");
            g_string_assign (newuri, fullpath->str);
        }
        else
            g_string_prepend (newuri, "http://");
        g_string_free (fullpath, TRUE);
    }
    /* if we do handle cookies, ask our handler for them */
    webkit_web_view_load_uri (uzbl.gui.web_view, newuri->str);
    g_string_free (newuri, TRUE);
}
