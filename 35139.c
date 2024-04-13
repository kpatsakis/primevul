find_existing_file(gchar* path_list) {
    int i=0;
    int cnt;
    gchar **split;
    gchar *tmp = NULL;
    gchar *executable;

    if(!path_list)
        return NULL;

    split = g_strsplit(path_list, ":", 0);
    while(split[i])
        i++;

    if(i<=1) {
        tmp = g_strdup(split[0]);
        g_strfreev(split);
        return tmp;
    }
    else
        cnt = i-1;

    i=0;
    tmp = g_strdup(split[cnt]);
    g_strstrip(tmp);
    if(tmp[0] == '/')
        executable = g_strdup_printf("%s", tmp+1);
    else
        executable = g_strdup(tmp);
    g_free(tmp);

    while(i<cnt) {
        tmp = g_strconcat(g_strstrip(split[i]), "/", executable, NULL);
        if(g_file_test(tmp, G_FILE_TEST_EXISTS)) {
            g_strfreev(split);
            return tmp;
        }
        else
            g_free(tmp);
        i++;
    }

    g_free(executable);
    g_strfreev(split);
    return NULL;
}
