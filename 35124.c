control_socket(GIOChannel *chan) {
    struct sockaddr_un remote;
    unsigned int t = sizeof(remote);
    GIOChannel *iochan;
    int clientsock;

    clientsock = accept (g_io_channel_unix_get_fd(chan),
                         (struct sockaddr *) &remote, &t);

    if(!uzbl.comm.client_chan)
        uzbl.comm.client_chan = g_ptr_array_new();

    if ((iochan = g_io_channel_unix_new(clientsock))) {
        g_io_channel_set_encoding(iochan, NULL, NULL);
        g_io_add_watch(iochan, G_IO_IN|G_IO_HUP,
                       (GIOFunc) control_client_socket, iochan);
        g_ptr_array_add(uzbl.comm.client_chan, (gpointer)iochan);
    }
    return TRUE;
}
