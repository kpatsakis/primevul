IRC_PROTOCOL_CALLBACK(part)
{
    char *pos_comment, *join_string;
    int join_length, local_part, display_host;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    struct t_irc_channel_speaking *ptr_nick_speaking;

    IRC_PROTOCOL_MIN_ARGS(3);
    IRC_PROTOCOL_CHECK_HOST;

    pos_comment = (argc > 3) ?
        ((argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]) : NULL;

    ptr_channel = irc_channel_search (server,
                                      (argv[2][0] == ':') ? argv[2] + 1 : argv[2]);
    if (!ptr_channel)
        return WEECHAT_RC_OK;

    ptr_nick = irc_nick_search (server, ptr_channel, nick);

    local_part = (irc_server_strcasecmp (server, nick, server->nick) == 0);

    /* display part message */
    if (!ignored)
    {
        ptr_nick_speaking = NULL;
        if (ptr_channel->type == IRC_CHANNEL_TYPE_CHANNEL)
        {
            ptr_nick_speaking = ((weechat_config_boolean (irc_config_look_smart_filter))
                                 && (weechat_config_boolean (irc_config_look_smart_filter_quit))) ?
                irc_channel_nick_speaking_time_search (server, ptr_channel, nick, 1) : NULL;
        }
        display_host = weechat_config_boolean (irc_config_look_display_host_quit);
        if (pos_comment)
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, NULL, ptr_channel->buffer),
                date,
                irc_protocol_tags (command,
                                   (local_part
                                    || (ptr_channel->type != IRC_CHANNEL_TYPE_CHANNEL)
                                    || !weechat_config_boolean (irc_config_look_smart_filter)
                                    || !weechat_config_boolean (irc_config_look_smart_filter_quit)
                                    || ptr_nick_speaking) ?
                                   NULL : "irc_smart_filter",
                                   nick, address),
                _("%s%s%s%s%s%s%s%s%s%s has left %s%s%s %s(%s%s%s)"),
                weechat_prefix ("quit"),
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_CHAT_DELIMITERS,
                (display_host) ? " (" : "",
                IRC_COLOR_CHAT_HOST,
                (display_host) ? address : "",
                IRC_COLOR_CHAT_DELIMITERS,
                (display_host) ? ")" : "",
                IRC_COLOR_MESSAGE_QUIT,
                IRC_COLOR_CHAT_CHANNEL,
                ptr_channel->name,
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
                irc_protocol_tags (command,
                                   (local_part
                                    || (ptr_channel->type != IRC_CHANNEL_TYPE_CHANNEL)
                                    || !weechat_config_boolean (irc_config_look_smart_filter)
                                    || !weechat_config_boolean (irc_config_look_smart_filter_quit)
                                    || ptr_nick_speaking) ?
                                   NULL : "irc_smart_filter",
                                   nick, address),
                _("%s%s%s%s%s%s%s%s%s%s has left %s%s%s"),
                weechat_prefix ("quit"),
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_CHAT_DELIMITERS,
                (display_host) ? " (" : "",
                IRC_COLOR_CHAT_HOST,
                (display_host) ? address : "",
                IRC_COLOR_CHAT_DELIMITERS,
                (display_host) ? ")" : "",
                IRC_COLOR_MESSAGE_QUIT,
                IRC_COLOR_CHAT_CHANNEL,
                ptr_channel->name,
                IRC_COLOR_MESSAGE_QUIT);
        }
    }

    /* part request was issued by local client ? */
    if (local_part)
    {
        irc_nick_free_all (server, ptr_channel);

        irc_channel_modelist_set_state (ptr_channel,
                                        IRC_MODELIST_STATE_MODIFIED);

        /* cycling ? => rejoin channel immediately */
        if (ptr_channel->cycle)
        {
            ptr_channel->cycle = 0;
            if (ptr_channel->key)
            {
                join_length = strlen (ptr_channel->name) + 1 +
                    strlen (ptr_channel->key) + 1;
                join_string = malloc (join_length);
                if (join_string)
                {
                    snprintf (join_string, join_length, "%s %s",
                              ptr_channel->name,
                              ptr_channel->key);
                    irc_command_join_server (server, join_string, 1, 1);
                    free (join_string);
                }
                else
                    irc_command_join_server (server, ptr_channel->name,
                                             1, 1);
            }
            else
                irc_command_join_server (server, ptr_channel->name,
                                         1, 1);
        }
        else
        {
            if (weechat_config_boolean (irc_config_look_part_closes_buffer))
                weechat_buffer_close (ptr_channel->buffer);
            else
                ptr_channel->part = 1;
        }
        irc_bar_item_update_channel ();
    }
    else if (ptr_nick)
    {
        /* part from another user */
        irc_channel_join_smart_filtered_remove (ptr_channel,
                                                ptr_nick->name);
        irc_nick_free (server, ptr_channel, ptr_nick);
    }

    return WEECHAT_RC_OK;
}