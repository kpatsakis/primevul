static gboolean on_server_socket_event(GIOChannel* ioc, GIOCondition cond, gpointer user_data)
{
    SingleInstData* data = user_data;

    if ( cond & (G_IO_IN|G_IO_PRI) )
    {
        int client_sock = accept(g_io_channel_unix_get_fd(ioc), NULL, 0);
        if(client_sock != -1)
        {
            SingleInstClient* client = g_slice_new0(SingleInstClient);
            client->channel = g_io_channel_unix_new(client_sock);
            g_io_channel_set_encoding(client->channel, NULL, NULL);
            client->screen_num = -1;
            client->argv = g_ptr_array_new();
            client->callback = data->cb;
            client->opt_entries = data->opt_entries;
            g_ptr_array_add(client->argv, g_strdup(g_get_prgname()));
            client->watch = g_io_add_watch(client->channel, G_IO_IN|G_IO_PRI|G_IO_ERR|G_IO_HUP,
                                           on_client_socket_event, client);
            clients = g_list_prepend(clients, client);
            /* g_debug("accept new client"); */
        }
        else
            g_debug("accept() failed!\n%s", g_strerror(errno));
    }

    if(cond & (G_IO_ERR|G_IO_HUP))
    {
        single_inst_finalize(data);
        single_inst_init(data);
        return FALSE;
    }

    return TRUE;
}
