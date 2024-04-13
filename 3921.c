IRC_PROTOCOL_CALLBACK(324)
{
    const char *ptr_modes, *ptr_modes_args;
    struct t_irc_channel *ptr_channel;

    IRC_PROTOCOL_MIN_ARGS(4);

    ptr_modes = (argc > 4) ?
        ((argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]) : NULL;
    ptr_modes_args = (argc > 5) ?
        ((argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5]) : NULL;

    ptr_channel = irc_channel_search (server, argv[3]);
    if (ptr_channel)
    {
        irc_channel_set_modes (ptr_channel, ptr_modes);
        if (argc > 4)
        {
            (void) irc_mode_channel_set (server, ptr_channel, host,
                                         ptr_modes, ptr_modes_args);
        }
    }
    if (!ptr_channel
        || (weechat_hashtable_has_key (ptr_channel->join_msg_received, command)
            || weechat_hashtable_has_key (irc_config_hashtable_display_join_message, command)))
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, NULL,
                (ptr_channel) ? ptr_channel->buffer : NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, address),
            _("%sMode %s%s %s[%s%s%s]"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (ptr_modes) ? ptr_modes : "",
            IRC_COLOR_CHAT_DELIMITERS);
    }

    if (ptr_channel)
        weechat_hashtable_set (ptr_channel->join_msg_received, command, "1");

    return WEECHAT_RC_OK;
}