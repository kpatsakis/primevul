parseenv (gchar* string) {
    extern char** environ;
    gchar* tmpstr = NULL, * out;
    int i = 0;

    if(!string)
        return NULL;

    out = g_strdup(string);
    while (environ[i] != NULL) {
        gchar** env = g_strsplit (environ[i], "=", 2);
        gchar* envname = g_strconcat ("$", env[0], NULL);

        if (g_strrstr (string, envname) != NULL) {
            tmpstr = out;
            out = str_replace(envname, env[1], out);
            g_free (tmpstr);
        }

        g_free (envname);
        g_strfreev (env); // somebody said this breaks uzbl
        i++;
    }

    return out;
}
