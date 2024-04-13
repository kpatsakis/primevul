void single_inst_finalize(SingleInstData* data)
{
    if(data->sock >=0)
    {
        close(data->sock);
        data->sock = -1;

        if(data->io_channel)
        {
            char sock_path[256];

            /* disconnect all clients */
            if(clients)
            {
                g_list_foreach(clients, (GFunc)single_inst_client_free, NULL);
                g_list_free(clients);
                clients = NULL;
            }

            if(data->io_watch)
            {
                g_source_remove(data->io_watch);
                data->io_watch = 0;
            }
            g_io_channel_unref(data->io_channel);
            data->io_channel = NULL;
            /* remove the file */
            get_socket_name(data, sock_path, 256);
            unlink(sock_path);
        }
    }
}
