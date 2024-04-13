IRC_PROTOCOL_CALLBACK(341)
{
    char *pos_channel;

    IRC_PROTOCOL_MIN_ARGS(5);

    pos_channel = (argv[4][0] == ':') ? argv[4] + 1 : argv[4];

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, argv[2], command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", argv[2], address),
        _("%s%s%s%s has invited %s%s%s to %s%s%s"),
        weechat_prefix ("network"),
        irc_nick_color_for_msg (server, 1, NULL, argv[2]),
        argv[2],
        IRC_COLOR_RESET,
        irc_nick_color_for_msg (server, 1, NULL, argv[3]),
        argv[3],
        IRC_COLOR_RESET,
        IRC_COLOR_CHAT_CHANNEL,
        pos_channel,
        IRC_COLOR_RESET);

    return WEECHAT_RC_OK;
}