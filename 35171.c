read_file_by_line (const gchar *path) {
    GIOChannel *chan = NULL;
    gchar *readbuf = NULL;
    gsize len;
    GArray *lines = g_array_new(TRUE, FALSE, sizeof(gchar*));
    int i = 0;

    chan = g_io_channel_new_file(path, "r", NULL);
    if (chan) {
        while (g_io_channel_read_line(chan, &readbuf, &len, NULL, NULL) == G_IO_STATUS_NORMAL) {
            const gchar* val = g_strdup (readbuf);
            g_array_append_val (lines, val);
            g_free (readbuf);
            i ++;
        }

        g_io_channel_unref (chan);
    } else {
        gchar *tmp = g_strdup_printf("File %s can not be read.", path);
        send_event(COMMAND_ERROR, tmp, NULL);
        g_free(tmp);
    }

    return lines;
}
