IRC_PROTOCOL_CALLBACK(329)
{
    struct t_irc_channel *ptr_channel;
    time_t datetime;

    IRC_PROTOCOL_MIN_ARGS(5);

    ptr_channel = irc_channel_search (server, argv[3]);

    datetime = (time_t)(atol ((argv_eol[4][0] == ':') ?
                              argv_eol[4] + 1 : argv_eol[4]));

    if (ptr_channel)
    {
        if (weechat_hashtable_has_key (ptr_channel->join_msg_received, command)
            || weechat_hashtable_has_key (irc_config_hashtable_display_join_message, command))
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, ptr_channel->buffer),
                date,
                irc_protocol_tags (command, "irc_numeric",
                                   NULL, NULL),
                /* TRANSLATORS: "%s" after "created on" is a date */
                _("%sChannel created on %s"),
                weechat_prefix ("network"),
                weechat_util_get_time_string (&datetime));
        }
    }
    else
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            /* TRANSLATORS: "%s" after "created on" is a date */
            _("%sChannel %s%s%s created on %s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_RESET,
            weechat_util_get_time_string (&datetime));
    }

    if (ptr_channel)
        weechat_hashtable_set (ptr_channel->join_msg_received, command, "1");

    return WEECHAT_RC_OK;
}