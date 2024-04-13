str_replace (const char* search, const char* replace, const char* string) {
    gchar **buf;
    char *ret;

    if(!string)
        return NULL;

    buf = g_strsplit (string, search, -1);
    ret = g_strjoinv (replace, buf);
    g_strfreev(buf);

    return ret;
}
