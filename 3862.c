IRC_PROTOCOL_CALLBACK(303)
{
    IRC_PROTOCOL_MIN_ARGS(4);

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        _("%sUsers online: %s%s"),
        weechat_prefix ("network"),
        IRC_COLOR_CHAT_NICK,
        (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]);

    return WEECHAT_RC_OK;
}