control_client_socket(GIOChannel *clientchan) {
    char *ctl_line;
    GString *result = g_string_new("");
    GError *error = NULL;
    GIOStatus ret;
    gsize len;

    ret = g_io_channel_read_line(clientchan, &ctl_line, &len, NULL, &error);
    if (ret == G_IO_STATUS_ERROR) {
        g_warning ("Error reading: %s\n", error->message);
        remove_socket_from_array(clientchan);
        g_io_channel_shutdown(clientchan, TRUE, &error);
        return FALSE;
    } else if (ret == G_IO_STATUS_EOF) {
        remove_socket_from_array(clientchan);
        /* shutdown and remove channel watch from main loop */
        g_io_channel_shutdown(clientchan, TRUE, &error);
        return FALSE;
    }

    if (ctl_line) {
        parse_cmd_line (ctl_line, result);
        g_string_append_c(result, '\n');
        ret = g_io_channel_write_chars (clientchan, result->str, result->len,
                                        &len, &error);
        if (ret == G_IO_STATUS_ERROR) {
            g_warning ("Error writing: %s", error->message);
        }
        g_io_channel_flush(clientchan, &error);
    }

    if (error) g_error_free (error);
    g_string_free(result, TRUE);
    g_free(ctl_line);
    return TRUE;
}
