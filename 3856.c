IRC_PROTOCOL_CALLBACK(301)
{
    char *pos_away_msg;
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(3);

    if (argc > 4)
    {
        pos_away_msg = (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4];

        /* look for private buffer to display message */
        ptr_channel = irc_channel_search (server, argv[3]);
        if (!weechat_config_boolean (irc_config_look_display_pv_away_once)
            || !ptr_channel
            || !(ptr_channel->away_message)
            || (strcmp (ptr_channel->away_message, pos_away_msg) != 0))
        {
            ptr_buffer = (ptr_channel) ? ptr_channel->buffer : server->buffer;
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, argv[3], command, "whois", ptr_buffer),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, address),
                _("%s%s[%s%s%s]%s is away: %s"),
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_DELIMITERS,
                irc_nick_color_for_msg (server, 1, NULL, argv[3]),
                argv[3],
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_RESET,
                pos_away_msg);
            if (ptr_channel)
            {
                if (ptr_channel->away_message)
                    free (ptr_channel->away_message);
                ptr_channel->away_message = strdup (pos_away_msg);
            }
        }
    }

    return WEECHAT_RC_OK;
}