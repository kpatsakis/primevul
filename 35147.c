init_connect_socket() {
    int sockfd, replay = 0;
    struct sockaddr_un local;
    GIOChannel *chan;
    gchar **name = NULL;

    if(!uzbl.comm.connect_chan)
        uzbl.comm.connect_chan = g_ptr_array_new();

    name = uzbl.state.connect_socket_names;

    while(name && *name) {
        sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
        local.sun_family = AF_UNIX;
        strcpy (local.sun_path, *name);

        if(!connect(sockfd, (struct sockaddr *) &local, sizeof(local))) {
            if ((chan = g_io_channel_unix_new(sockfd))) {
                g_io_channel_set_encoding(chan, NULL, NULL);
                g_io_add_watch(chan, G_IO_IN|G_IO_HUP,
                        (GIOFunc) control_client_socket, chan);
                g_ptr_array_add(uzbl.comm.connect_chan, (gpointer)chan);
                replay++;
            }
        }
        else
            g_warning("Error connecting to socket: %s\n", *name);
        name++;
    }

    /* replay buffered events */
    if(replay)
        send_event_socket(NULL);
}
