IRC_PROTOCOL_CALLBACK(quit)
{
    char *pos_comment;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    struct t_irc_channel_speaking *ptr_nick_speaking;
    int local_quit, display_host;

    IRC_PROTOCOL_MIN_ARGS(2);
    IRC_PROTOCOL_CHECK_HOST;

    pos_comment = (argc > 2) ?
        ((argv_eol[2][0] == ':') ? argv_eol[2] + 1 : argv_eol[2]) : NULL;

    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        if (ptr_channel->type == IRC_CHANNEL_TYPE_PRIVATE)
            ptr_nick = NULL;
        else
            ptr_nick = irc_nick_search (server, ptr_channel, nick);

        if (ptr_nick
            || (irc_server_strcasecmp (server, ptr_channel->name, nick) == 0))
        {
            local_quit = (irc_server_strcasecmp (server, nick, server->nick) == 0);
            if (!irc_ignore_check (server, ptr_channel->name, nick, host))
            {
                /* display quit message */
                ptr_nick_speaking = NULL;
                if (ptr_channel->type == IRC_CHANNEL_TYPE_CHANNEL)
                {
                    ptr_nick_speaking = ((weechat_config_boolean (irc_config_look_smart_filter))
                                         && (weechat_config_boolean (irc_config_look_smart_filter_quit))) ?
                        irc_channel_nick_speaking_time_search (server, ptr_channel, nick, 1) : NULL;
                }
                if (ptr_channel->type == IRC_CHANNEL_TYPE_PRIVATE)
                {
                    ptr_channel->has_quit_server = 1;
                }
                display_host = weechat_config_boolean (irc_config_look_display_host_quit);
                if (pos_comment && pos_comment[0])
                {
                    weechat_printf_date_tags (
                        irc_msgbuffer_get_target_buffer (
                            server, NULL, command, NULL, ptr_channel->buffer),
                        date,
                        irc_protocol_tags (
                            command,
                            (local_quit
                             || (ptr_channel->type != IRC_CHANNEL_TYPE_CHANNEL)
                             || !weechat_config_boolean (irc_config_look_smart_filter)
                             || !weechat_config_boolean (irc_config_look_smart_filter_quit)
                             || ptr_nick_speaking) ?
                            NULL : "irc_smart_filter",
                            nick, address),
                        _("%s%s%s%s%s%s%s%s%s%s has quit %s(%s%s%s)"),
                        weechat_prefix ("quit"),
                        (ptr_channel->type == IRC_CHANNEL_TYPE_PRIVATE) ?
                        irc_nick_color_for_pv (ptr_channel, nick) : irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                        nick,
                        IRC_COLOR_CHAT_DELIMITERS,
                        (display_host) ? " (" : "",
                        IRC_COLOR_CHAT_HOST,
                        (display_host) ? address : "",
                        IRC_COLOR_CHAT_DELIMITERS,
                        (display_host) ? ")" : "",
                        IRC_COLOR_MESSAGE_QUIT,
                        IRC_COLOR_CHAT_DELIMITERS,
                        IRC_COLOR_REASON_QUIT,
                        pos_comment,
                        IRC_COLOR_CHAT_DELIMITERS);
                }
                else
                {
                    weechat_printf_date_tags (
                        irc_msgbuffer_get_target_buffer (
                            server, NULL, command, NULL, ptr_channel->buffer),
                        date,
                        irc_protocol_tags (
                            command,
                            (local_quit
                             || (ptr_channel->type != IRC_CHANNEL_TYPE_CHANNEL)
                             || !weechat_config_boolean (irc_config_look_smart_filter)
                             || !weechat_config_boolean (irc_config_look_smart_filter_quit)
                             || ptr_nick_speaking) ?
                            NULL : "irc_smart_filter",
                            nick, address),
                        _("%s%s%s%s%s%s%s%s%s%s has quit"),
                        weechat_prefix ("quit"),
                        (ptr_channel->type == IRC_CHANNEL_TYPE_PRIVATE) ?
                        irc_nick_color_for_pv (ptr_channel, nick) : irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                        nick,
                        IRC_COLOR_CHAT_DELIMITERS,
                        (display_host) ? " (" : "",
                        IRC_COLOR_CHAT_HOST,
                        (display_host) ? address : "",
                        IRC_COLOR_CHAT_DELIMITERS,
                        (display_host) ? ")" : "",
                        IRC_COLOR_MESSAGE_QUIT);
                }
            }
            if (!local_quit && ptr_nick)
            {
                irc_channel_join_smart_filtered_remove (ptr_channel,
                                                        ptr_nick->name);
            }
            if (ptr_nick)
                irc_nick_free (server, ptr_channel, ptr_nick);
        }
    }

    return WEECHAT_RC_OK;
}