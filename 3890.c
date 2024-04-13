IRC_PROTOCOL_CALLBACK(kill)
{
    char *pos_comment;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick, *ptr_nick_killed;

    IRC_PROTOCOL_MIN_ARGS(3);
    IRC_PROTOCOL_CHECK_HOST;

    pos_comment = (argc > 3) ?
        ((argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]) : NULL;

    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        ptr_nick = irc_nick_search (server, ptr_channel, nick);
        ptr_nick_killed = irc_nick_search (server, ptr_channel, argv[2]);

        if (pos_comment)
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (server, NULL, command, NULL,
                                                 ptr_channel->buffer),
                date,
                irc_protocol_tags (command, NULL, NULL, address),
                _("%s%sYou were killed by %s%s%s %s(%s%s%s)"),
                weechat_prefix ("quit"),
                IRC_COLOR_MESSAGE_KICK,
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_MESSAGE_KICK,
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_REASON_KICK,
                pos_comment,
                IRC_COLOR_CHAT_DELIMITERS);
        }
        else
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (server, NULL, command, NULL,
                                                 ptr_channel->buffer),
                date,
                irc_protocol_tags (command, NULL, NULL, address),
                _("%s%sYou were killed by %s%s%s"),
                weechat_prefix ("quit"),
                IRC_COLOR_MESSAGE_KICK,
                irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                nick,
                IRC_COLOR_MESSAGE_KICK);
        }

        if (irc_server_strcasecmp (server, argv[2], server->nick) == 0)
        {
            /*
             * my nick was killed => free all nicks, channel is not active any
             * more
             */
            irc_nick_free_all (server, ptr_channel);

            irc_channel_modelist_set_state (ptr_channel,
                                            IRC_MODELIST_STATE_MODIFIED);

            irc_bar_item_update_channel ();
        }
        else
        {
            /*
             * someone was killed on channel (but not me) => remove only this
             * nick
             */
            if (ptr_nick_killed)
                irc_nick_free (server, ptr_channel, ptr_nick_killed);
        }
    }

    return WEECHAT_RC_OK;
}