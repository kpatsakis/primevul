IRC_PROTOCOL_CALLBACK(328)
{
    struct t_irc_channel *ptr_channel;

    IRC_PROTOCOL_MIN_ARGS(5);

    ptr_channel = irc_channel_search (server, argv[3]);
    if (ptr_channel)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, NULL, ptr_channel->buffer),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            _("%sURL for %s%s%s: %s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_RESET,
            (argv_eol[4][0] == ':') ?
            argv_eol[4] + 1 : argv_eol[4]);
    }

    return WEECHAT_RC_OK;
}