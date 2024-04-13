IRC_PROTOCOL_CALLBACK(008)
{
    IRC_PROTOCOL_MIN_ARGS(4);

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, argv[2], command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, address),
        _("%sServer notice mask for %s%s%s: %s"),
        weechat_prefix ("network"),
        irc_nick_color_for_msg (server, 1, NULL, argv[2]),
        argv[2],
        IRC_COLOR_RESET,
        (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]);

    return WEECHAT_RC_OK;
}