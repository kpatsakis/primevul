IRC_PROTOCOL_CALLBACK(345)
{
    IRC_PROTOCOL_MIN_ARGS(5);

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, NULL, command, "reop", NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s%s%s: %s",
        weechat_prefix ("network"),
        IRC_COLOR_CHAT_CHANNEL,
        argv[3],
        IRC_COLOR_RESET,
        (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]);

    return WEECHAT_RC_OK;
}