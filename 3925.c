IRC_PROTOCOL_CALLBACK(mode)
{
    char *pos_modes, *pos_modes_args, *modes_args;
    int smart_filter, local_mode;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(4);
    IRC_PROTOCOL_CHECK_HOST;

    pos_modes = (argv[3][0] == ':') ? argv[3] + 1 : argv[3];
    pos_modes_args = (argc > 4) ?
        ((argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]) : NULL;

    if (irc_channel_is_channel (server, argv[2]))
    {
        smart_filter = 0;
        ptr_channel = irc_channel_search (server, argv[2]);
        if (ptr_channel)
        {
            smart_filter = irc_mode_channel_set (server, ptr_channel, host,
                                                 pos_modes, pos_modes_args);
        }
        local_mode = (irc_server_strcasecmp (server, nick, server->nick) == 0);
        ptr_nick = irc_nick_search (server, ptr_channel, nick);
        ptr_buffer = (ptr_channel) ? ptr_channel->buffer : server->buffer;
        modes_args = irc_mode_get_arguments (pos_modes_args);
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL,
                                             ptr_buffer),
            date,
            irc_protocol_tags (command,
                               (smart_filter && !local_mode) ?
                               "irc_smart_filter" : NULL,
                               NULL, address),
            _("%sMode %s%s %s[%s%s%s%s%s]%s by %s%s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            (ptr_channel) ? ptr_channel->name : argv[2],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            pos_modes,
            (modes_args && modes_args[0]) ? " " : "",
            (modes_args && modes_args[0]) ? modes_args : "",
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            irc_nick_color_for_msg (server, 1, ptr_nick, nick),
            nick);
        if (modes_args)
            free (modes_args);
    }
    else
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
            date,
            irc_protocol_tags (command, NULL, NULL, address),
            _("%sUser mode %s[%s%s%s]%s by %s%s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            pos_modes,
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            irc_nick_color_for_msg (server, 1, NULL, nick),
            nick);
        irc_mode_user_set (server, pos_modes, 0);
    }

    return WEECHAT_RC_OK;
}