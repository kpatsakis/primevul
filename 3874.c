IRC_PROTOCOL_CALLBACK(221)
{
    IRC_PROTOCOL_MIN_ARGS(4);

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, argv[2], command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, address),
        _("%sUser mode for %s%s%s is %s[%s%s%s]"),
        weechat_prefix ("network"),
        irc_nick_color_for_msg (server, 1, NULL, argv[2]),
        argv[2],
        IRC_COLOR_RESET,
        IRC_COLOR_CHAT_DELIMITERS,
        IRC_COLOR_RESET,
        (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3],
        IRC_COLOR_CHAT_DELIMITERS);

    if (irc_server_strcasecmp (server, argv[2], server->nick) == 0)
    {
        irc_mode_user_set (
            server,
            (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3],
            1);
    }

    return WEECHAT_RC_OK;
}