IRC_PROTOCOL_CALLBACK(327)
{
    char *pos_realname;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(6);

    pos_realname = (argc > 6) ?
        ((argv_eol[6][0] == ':') ? argv_eol[6] + 1 : argv_eol[6]) : NULL;

    ptr_buffer = irc_msgbuffer_get_target_buffer (server, argv[3],
                                                  command, "whois", NULL);

    if (pos_realname && pos_realname[0])
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s[%s%s%s] %s%s %s %s(%s%s%s)",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[3]),
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_CHAT_HOST,
            argv[4],
            argv[5],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            pos_realname,
            IRC_COLOR_CHAT_DELIMITERS);
    }
    else
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s[%s%s%s] %s%s %s",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[3]),
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_CHAT_HOST,
            argv[4],
            argv[5]);
    }

    return WEECHAT_RC_OK;
}