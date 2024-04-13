static gboolean on_client_socket_event(GIOChannel* ioc, GIOCondition cond, gpointer user_data)
{
    SingleInstClient* client = (SingleInstClient*)user_data;

    if ( cond & (G_IO_IN|G_IO_PRI) )
    {
        GString *str = g_string_sized_new(1024);
        gsize got;
        gchar ch;
        GIOStatus status;

        while((status = g_io_channel_read_chars(ioc, &ch, 1, &got, NULL)) == G_IO_STATUS_NORMAL)
        {
            if(ch != '\n')
            {
                if(ch < 0x20) /* zero or control char */
                {
                    g_error("client connection: invalid char %#x", (int)ch);
                    break;
                }
                g_string_append_c(str, ch);
                continue;
            }
            if(str->len)
            {
                char *line = g_strndup(str->str, str->len);

                g_string_truncate(str, 0);
                g_debug("line = %s", line);
                if(!client->cwd)
                    client->cwd = g_strcompress(line);
                else if(client->screen_num == -1)
                {
                    client->screen_num = atoi(line);
                    if(client->screen_num < 0)
                        client->screen_num = 0;
                }
                else
                {
                    char* str = g_strcompress(line);
                    g_ptr_array_add(client->argv, str);
                }
                g_free(line);
            }
        }
        g_string_free(str, TRUE);
        switch(status)
        {
            case G_IO_STATUS_ERROR:
                cond |= G_IO_ERR;
                break;
            case G_IO_STATUS_EOF:
                cond |= G_IO_HUP;
            default:
                break;
        }
    }

    if(cond & (G_IO_ERR|G_IO_HUP))
    {
        if(! (cond & G_IO_ERR) ) /* if there is no error */
        {
            /* try to parse argv */
            parse_args(client);
        }
        clients = g_list_remove(clients, client);
        single_inst_client_free(client);
        return FALSE;
    }

    return TRUE;
}
