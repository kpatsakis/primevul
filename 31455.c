SingleInstResult single_inst_init(SingleInstData* data)
{
    struct sockaddr_un addr;
    int addr_len;
    int ret;
    int reuse;

    data->io_channel = NULL;
    data->io_watch = 0;
    if((data->sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        return SINGLE_INST_ERROR;

    /* FIXME: use abstract socket? */
    addr.sun_family = AF_UNIX;
    get_socket_name(data, addr.sun_path, sizeof(addr.sun_path));
#ifdef SUN_LEN
    addr_len = SUN_LEN(&addr);
#else
    addr_len = strlen(addr.sun_path) + sizeof(addr.sun_family);
#endif

    /* try to connect to existing instance */
    if(connect(data->sock, (struct sockaddr*)&addr, addr_len) == 0)
    {
        /* connected successfully, pass args in opt_entries to server process as argv and exit. */
        pass_args_to_existing_instance(data->opt_entries, data->screen_num, data->sock);
        return SINGLE_INST_CLIENT;
    }

    /* There is no existing server, and we are in the first instance. */
    unlink(addr.sun_path); /* delete old socket file if it exists. */

    reuse = 1;
    ret = setsockopt( data->sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse) );
    if(ret || bind(data->sock, (struct sockaddr*)&addr, addr_len) == -1)
        return SINGLE_INST_ERROR;

    data->io_channel = g_io_channel_unix_new(data->sock);
    if(data->io_channel == NULL)
        return SINGLE_INST_ERROR;

    g_io_channel_set_encoding(data->io_channel, NULL, NULL);
    g_io_channel_set_buffered(data->io_channel, FALSE);

    if(listen(data->sock, 5) == -1)
        return SINGLE_INST_ERROR;

    data->io_watch = g_io_add_watch(data->io_channel,
                                    G_IO_IN|G_IO_ERR|G_IO_PRI|G_IO_HUP,
                                    (GIOFunc)on_server_socket_event, data);
    return SINGLE_INST_SERVER;
}
