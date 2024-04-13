IRC_PROTOCOL_CALLBACK(338)
{
    IRC_PROTOCOL_MIN_ARGS(6);

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, argv[3], command, "whois", NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s[%s%s%s]%s %s %s%s",
        weechat_prefix ("network"),
        IRC_COLOR_CHAT_DELIMITERS,
        irc_nick_color_for_msg (server, 1, NULL, argv[3]),
        argv[3],
        IRC_COLOR_CHAT_DELIMITERS,
        IRC_COLOR_RESET,
        (argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5],
        IRC_COLOR_CHAT_HOST,
        argv[4]);

    return WEECHAT_RC_OK;
}