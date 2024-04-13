IRC_PROTOCOL_CALLBACK(315)
{
    struct t_irc_channel *ptr_channel;

    IRC_PROTOCOL_MIN_ARGS(5);

    ptr_channel = irc_channel_search (server, argv[3]);
    if (ptr_channel && (ptr_channel->checking_whox > 0))
    {
        ptr_channel->checking_whox--;
    }
    else
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "who", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s[%s%s%s]%s %s",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]);
    }

    return WEECHAT_RC_OK;
}