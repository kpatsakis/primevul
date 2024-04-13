IRC_PROTOCOL_CALLBACK(367)
{
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;
    struct t_irc_modelist *ptr_modelist;
    time_t datetime;
    const char *nick_address;
    char str_number[64];

    IRC_PROTOCOL_MIN_ARGS(5);

    ptr_channel = irc_channel_search (server, argv[3]);
    ptr_buffer = (ptr_channel && ptr_channel->nicks) ?
        ptr_channel->buffer : server->buffer;
    ptr_modelist = irc_modelist_search (ptr_channel, 'b');

    if (ptr_modelist)
    {
        /* start receiving new list */
        if (ptr_modelist->state != IRC_MODELIST_STATE_RECEIVING)
        {
            irc_modelist_item_free_all (ptr_modelist);
            ptr_modelist->state = IRC_MODELIST_STATE_RECEIVING;
        }

        snprintf (str_number, sizeof (str_number),
                  "%s[%s%d%s] ",
                  IRC_COLOR_CHAT_DELIMITERS,
                  IRC_COLOR_RESET,
                  ((ptr_modelist->last_item) ? ptr_modelist->last_item->number + 1 : 0) + 1,
                  IRC_COLOR_CHAT_DELIMITERS);
    }
    else
        str_number[0] = '\0';

    if (argc >= 6)
    {
        nick_address = irc_protocol_nick_address (
            server, 1, NULL, irc_message_get_nick_from_host (argv[5]),
            irc_message_get_address_from_host (argv[5]));
        if (argc >= 7)
        {
            datetime = (time_t)(atol ((argv[6][0] == ':') ? argv[6] + 1 : argv[6]));
            if (ptr_modelist)
                irc_modelist_item_new (ptr_modelist, argv[4], argv[5], datetime);
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, "banlist", ptr_buffer),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                /* TRANSLATORS: "%s" after "on" is a date */
                _("%s%s[%s%s%s] %s%s%s%s banned by %s on %s"),
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_CHAT_CHANNEL,
                argv[3],
                IRC_COLOR_CHAT_DELIMITERS,
                str_number,
                IRC_COLOR_CHAT_HOST,
                argv[4],
                IRC_COLOR_RESET,
                (nick_address[0]) ? nick_address : "?",
                weechat_util_get_time_string (&datetime));
        }
        else
        {
            if (ptr_modelist)
                irc_modelist_item_new (ptr_modelist, argv[4], argv[5], 0);
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, "banlist", ptr_buffer),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                _("%s%s[%s%s%s] %s%s%s%s banned by %s"),
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_CHAT_CHANNEL,
                argv[3],
                IRC_COLOR_CHAT_DELIMITERS,
                str_number,
                IRC_COLOR_CHAT_HOST,
                argv[4],
                IRC_COLOR_RESET,
                (nick_address[0]) ? nick_address : "?");
        }
    }
    else
    {
        if (ptr_modelist)
            irc_modelist_item_new (ptr_modelist, argv[4], NULL, 0);
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "banlist", ptr_buffer),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            _("%s%s[%s%s%s] %s%s%s%s banned"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            str_number,
            IRC_COLOR_CHAT_HOST,
            argv[4],
            IRC_COLOR_RESET);
    }

    return WEECHAT_RC_OK;
}