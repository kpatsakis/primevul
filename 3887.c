IRC_PROTOCOL_CALLBACK(330_343)
{
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(5);

    if (argc >= 6)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, argv[3], command, "whois", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s[%s%s%s] %s%s %s%s",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[3]),
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5],
            irc_nick_color_for_msg (server, 1, NULL, argv[4]),
            argv[4]);
    }
    else
    {
        ptr_channel = (irc_channel_is_channel (server, argv[3])) ?
            irc_channel_search (server, argv[3]) : NULL;
        ptr_buffer = (ptr_channel) ? ptr_channel->buffer : server->buffer;
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, argv[3], command, "whois", ptr_buffer),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s[%s%s%s] %s%s",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[3]),
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]);
    }

    return WEECHAT_RC_OK;
}