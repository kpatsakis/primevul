static gboolean lxterminal_socket_accept_client(GIOChannel * source, GIOCondition condition, LXTermWindow * lxtermwin)
{
    if (condition & G_IO_IN)
    {
        /* Accept the new connection. */
        int fd = accept(g_io_channel_unix_get_fd(source), NULL, NULL);
        if (fd < 0)
            g_warning("Accept failed: %s\n", g_strerror(errno));

        /* Add O_NONBLOCK to the flags. */
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

        /* Create a glib I/O channel. */
        GIOChannel * gio = g_io_channel_unix_new(fd);
        if (gio == NULL)
            g_warning("Cannot create new GIOChannel\n");
        else
        {
            /* Set up the glib I/O channel and add it to the event loop. */
            g_io_channel_set_encoding(gio, NULL, NULL);
            g_io_add_watch(gio, G_IO_IN | G_IO_HUP, (GIOFunc) lxterminal_socket_read_channel, lxtermwin);
            g_io_channel_unref(gio);
        }
    }

    /* Our listening socket hung up - we are dead. */
    if (condition & G_IO_HUP)
        g_error("Server listening socket closed unexpectedly\n");

    return TRUE;
}
