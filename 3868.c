IRC_PROTOCOL_CALLBACK(900)
{
    IRC_PROTOCOL_MIN_ARGS(6);

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, argv[3], command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s %s(%s%s%s)",
        weechat_prefix ("network"),
        (argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5],
        IRC_COLOR_CHAT_DELIMITERS,
        IRC_COLOR_CHAT_HOST,
        argv[3],
        IRC_COLOR_CHAT_DELIMITERS);

    return WEECHAT_RC_OK;
}