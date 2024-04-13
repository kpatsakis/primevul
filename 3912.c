IRC_PROTOCOL_CALLBACK(topic)
{
    char *pos_topic, *old_topic_color, *topic_color;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(3);

    if (!irc_channel_is_channel (server, argv[2]))
    {
        weechat_printf (server->buffer,
                        _("%s%s: \"%s\" command received without channel"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME, "topic");
        return WEECHAT_RC_OK;
    }

    pos_topic = (argc > 3) ?
        ((argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]) : NULL;

    ptr_channel = irc_channel_search (server, argv[2]);
    ptr_nick = irc_nick_search (server, ptr_channel, nick);
    ptr_buffer = (ptr_channel) ? ptr_channel->buffer : server->buffer;

    /*
     * unmask a smart filtered join if it is in hashtable
     * "join_smart_filtered" of channel
     */
    if (ptr_channel)
        irc_channel_join_smart_filtered_unmask (ptr_channel, nick);

    if (pos_topic && pos_topic[0])
    {
        topic_color = irc_color_decode (
            pos_topic,
            weechat_config_boolean (irc_config_network_colors_receive));
        if (weechat_config_boolean (irc_config_look_display_old_topic)
            && ptr_channel && ptr_channel->topic && ptr_channel->topic[0])
        {
            old_topic_color = irc_color_decode (
                ptr_channel->topic,
                weechat_config_boolean (irc_config_network_colors_receive));
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, ptr_buffer),
                date,
                irc_protocol_tags (command, NULL, NULL, address),
                _("%s%s%s%s has changed topic for %s%s%s from \"%s%s%s\" to "
                  "\"%s%s%s\""),
                weechat_prefix ("network"),
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_RESET,
                IRC_COLOR_CHAT_CHANNEL,
                argv[2],
                IRC_COLOR_RESET,
                IRC_COLOR_TOPIC_OLD,
                (old_topic_color) ? old_topic_color : ptr_channel->topic,
                IRC_COLOR_RESET,
                IRC_COLOR_TOPIC_NEW,
                (topic_color) ? topic_color : pos_topic,
                IRC_COLOR_RESET);
            if (old_topic_color)
                free (old_topic_color);
        }
        else
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, ptr_buffer),
                date,
                irc_protocol_tags (command, NULL, NULL, address),
                _("%s%s%s%s has changed topic for %s%s%s to \"%s%s%s\""),
                weechat_prefix ("network"),
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_RESET,
                IRC_COLOR_CHAT_CHANNEL,
                argv[2],
                IRC_COLOR_RESET,
                IRC_COLOR_TOPIC_NEW,
                (topic_color) ? topic_color : pos_topic,
                IRC_COLOR_RESET);
        }
        if (topic_color)
            free (topic_color);
    }
    else
    {
        if (weechat_config_boolean (irc_config_look_display_old_topic)
            && ptr_channel && ptr_channel->topic && ptr_channel->topic[0])
        {
            old_topic_color = irc_color_decode (
                ptr_channel->topic,
                weechat_config_boolean (irc_config_network_colors_receive));
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, ptr_buffer),
                date,
                irc_protocol_tags (command, NULL, NULL, address),
                _("%s%s%s%s has unset topic for %s%s%s (old topic: "
                  "\"%s%s%s\")"),
                weechat_prefix ("network"),
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_RESET,
                IRC_COLOR_CHAT_CHANNEL,
                argv[2],
                IRC_COLOR_RESET,
                IRC_COLOR_TOPIC_OLD,
                (old_topic_color) ? old_topic_color : ptr_channel->topic,
                IRC_COLOR_RESET);
            if (old_topic_color)
                free (old_topic_color);
        }
        else
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, ptr_buffer),
                date,
                irc_protocol_tags (command, NULL, NULL, address),
                _("%s%s%s%s has unset topic for %s%s%s"),
                weechat_prefix ("network"),
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_RESET,
                IRC_COLOR_CHAT_CHANNEL,
                argv[2],
                IRC_COLOR_RESET);
        }
    }

    if (ptr_channel)
        irc_channel_set_topic (ptr_channel, pos_topic);

    return WEECHAT_RC_OK;
}