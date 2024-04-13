IRC_PROTOCOL_CALLBACK(333)
{
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    time_t datetime;
    const char *topic_nick, *topic_address;
    int arg_date;

    IRC_PROTOCOL_MIN_ARGS(5);

    topic_nick = (argc > 5) ? irc_message_get_nick_from_host (argv[4]) : NULL;
    topic_address = (argc > 5) ? irc_message_get_address_from_host (argv[4]) : NULL;
    if (topic_nick && topic_address && strcmp (topic_nick, topic_address) == 0)
        topic_address = NULL;

    ptr_channel = irc_channel_search (server, argv[3]);
    ptr_nick = (ptr_channel) ?
        irc_nick_search (server, ptr_channel, topic_nick) : NULL;
    arg_date = (argc > 5) ? 5 : 4;
    datetime = (time_t)(atol ((argv_eol[arg_date][0] == ':') ?
                              argv_eol[arg_date] + 1 : argv_eol[arg_date]));

    if (!topic_nick && (datetime == 0))
        return WEECHAT_RC_OK;

    if (ptr_channel && ptr_channel->nicks)
    {
        if (weechat_hashtable_has_key (ptr_channel->join_msg_received, command)
            || weechat_hashtable_has_key (irc_config_hashtable_display_join_message, command))
        {
            if (topic_nick)
            {
                weechat_printf_date_tags (
                    irc_msgbuffer_get_target_buffer (
                        server, NULL, command, NULL, ptr_channel->buffer),
                    date,
                    irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                    /* TRANSLATORS: "%s" after "on" is a date */
                    _("%sTopic set by %s%s%s%s%s%s%s%s%s on %s"),
                    weechat_prefix ("network"),
                    irc_nick_color_for_msg (server, 1, ptr_nick, topic_nick),
                    topic_nick,
                    IRC_COLOR_CHAT_DELIMITERS,
                    (topic_address && topic_address[0]) ? " (" : "",
                    IRC_COLOR_CHAT_HOST,
                    (topic_address) ? topic_address : "",
                    IRC_COLOR_CHAT_DELIMITERS,
                    (topic_address && topic_address[0]) ? ")" : "",
                    IRC_COLOR_RESET,
                    weechat_util_get_time_string (&datetime));
            }
            else
            {
                weechat_printf_date_tags (
                    irc_msgbuffer_get_target_buffer (
                        server, NULL, command, NULL, ptr_channel->buffer),
                    date,
                    irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                    /* TRANSLATORS: "%s" after "on" is a date */
                    _("%sTopic set on %s"),
                    weechat_prefix ("network"),
                    weechat_util_get_time_string (&datetime));
            }
        }
    }
    else
    {
        if (topic_nick)
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, NULL),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                /* TRANSLATORS: "%s" after "on" is a date */
                _("%sTopic for %s%s%s set by %s%s%s%s%s%s%s%s%s on %s"),
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_CHANNEL,
                argv[3],
                IRC_COLOR_RESET,
                irc_nick_color_for_msg (server, 1, ptr_nick, topic_nick),
                topic_nick,
                IRC_COLOR_CHAT_DELIMITERS,
                (topic_address && topic_address[0]) ? " (" : "",
                IRC_COLOR_CHAT_HOST,
                (topic_address) ? topic_address : "",
                IRC_COLOR_CHAT_DELIMITERS,
                (topic_address && topic_address[0]) ? ")" : "",
                IRC_COLOR_RESET,
                weechat_util_get_time_string (&datetime));
        }
        else
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, NULL),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                /* TRANSLATORS: "%s" after "on" is a date */
                _("%sTopic for %s%s%s set on %s"),
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_CHANNEL,
                argv[3],
                IRC_COLOR_RESET,
                weechat_util_get_time_string (&datetime));
        }
    }

    if (ptr_channel)
        weechat_hashtable_set (ptr_channel->join_msg_received, command, "1");

    return WEECHAT_RC_OK;
}