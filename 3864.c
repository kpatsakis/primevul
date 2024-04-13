IRC_PROTOCOL_CALLBACK(332)
{
    char *pos_topic, *topic_no_color, *topic_color;
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(4);

    pos_topic = NULL;
    if (argc >= 5)
        pos_topic = (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4];

    ptr_channel = irc_channel_search (server, argv[3]);

    if (ptr_channel && ptr_channel->nicks)
    {
        if (pos_topic)
        {
            topic_no_color = (weechat_config_boolean (irc_config_network_colors_receive)) ?
                NULL : irc_color_decode (pos_topic, 0);
            irc_channel_set_topic (ptr_channel,
                                   (topic_no_color) ? topic_no_color : pos_topic);
            if (topic_no_color)
                free (topic_no_color);
        }
        ptr_buffer = ptr_channel->buffer;
    }
    else
        ptr_buffer = server->buffer;

    topic_color = NULL;
    if (pos_topic)
    {
        topic_color = irc_color_decode (pos_topic,
                                        (weechat_config_boolean (irc_config_network_colors_receive)) ? 1 : 0);
    }

    if (!ptr_channel
        || (weechat_hashtable_has_key (ptr_channel->join_msg_received, command))
        || weechat_hashtable_has_key (irc_config_hashtable_display_join_message, command))
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, NULL, ptr_buffer),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            _("%sTopic for %s%s%s is \"%s%s%s\""),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_RESET,
            IRC_COLOR_TOPIC_CURRENT,
            (topic_color) ? topic_color : ((pos_topic) ? pos_topic : ""),
            IRC_COLOR_RESET);
    }

    if (topic_color)
        free (topic_color);

    if (ptr_channel)
        weechat_hashtable_set (ptr_channel->join_msg_received, command, "1");

    return WEECHAT_RC_OK;
}