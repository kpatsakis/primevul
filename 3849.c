IRC_PROTOCOL_CALLBACK(470)
{
    struct t_gui_buffer *ptr_buffer;
    struct t_gui_lines *own_lines;
    const char *buffer_name, *short_name, *localvar_channel;
    char *old_channel_lower, *new_channel_lower;
    int lines_count;

    irc_protocol_cb_generic_error (server,
                                   date, nick, address, host, command,
                                   ignored, argc, argv, argv_eol);

    if ((argc >= 5) && !irc_channel_search (server, argv[3]))
    {
        ptr_buffer = irc_channel_search_buffer (server,
                                                IRC_CHANNEL_TYPE_CHANNEL,
                                                argv[3]);
        if (ptr_buffer)
        {
            short_name = weechat_buffer_get_string (ptr_buffer, "short_name");
            localvar_channel = weechat_buffer_get_string (ptr_buffer,
                                                          "localvar_channel");
            if (!short_name
                || (localvar_channel
                    && (strcmp (localvar_channel, short_name) == 0)))
            {
                /*
                 * update the short_name only if it was not changed by the
                 * user
                 */
                weechat_buffer_set (ptr_buffer, "short_name", argv[4]);
            }
            buffer_name = irc_buffer_build_name (server->name, argv[4]);
            weechat_buffer_set (ptr_buffer, "name", buffer_name);
            weechat_buffer_set (ptr_buffer, "localvar_set_channel", argv[4]);

            /*
             * check if logger backlog should be displayed for the new channel
             * name: it is displayed only if the buffer is currently completely
             * empty (no messages at all)
             */
            lines_count = 0;
            own_lines = weechat_hdata_pointer (weechat_hdata_get ("buffer"),
                                               ptr_buffer, "own_lines");
            if (own_lines)
            {
                lines_count = weechat_hdata_integer (
                    weechat_hdata_get ("lines"),
                    own_lines, "lines_count");
            }
            if (lines_count == 0)
            {
                (void) weechat_hook_signal_send ("logger_backlog",
                                                 WEECHAT_HOOK_SIGNAL_POINTER,
                                                 ptr_buffer);
            }
        }


        old_channel_lower = strdup (argv[3]);
        if (old_channel_lower)
        {
            weechat_string_tolower (old_channel_lower);
            new_channel_lower = strdup (argv[4]);
            if (new_channel_lower)
            {
                weechat_string_tolower (new_channel_lower);

                if (weechat_hashtable_has_key (server->join_manual,
                                               old_channel_lower))
                {
                    weechat_hashtable_set (server->join_manual,
                                           new_channel_lower,
                                           weechat_hashtable_get (
                                               server->join_manual,
                                               old_channel_lower));
                    weechat_hashtable_remove (server->join_manual,
                                              old_channel_lower);
                }

                if (weechat_hashtable_has_key (server->join_noswitch,
                                               old_channel_lower))
                {
                    weechat_hashtable_set (server->join_noswitch,
                                           new_channel_lower,
                                           weechat_hashtable_get (
                                               server->join_noswitch,
                                               old_channel_lower));
                    weechat_hashtable_remove (server->join_noswitch,
                                              old_channel_lower);
                }

                free (new_channel_lower);
            }

            free (old_channel_lower);
        }
    }

    return WEECHAT_RC_OK;
}