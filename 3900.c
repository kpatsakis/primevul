IRC_PROTOCOL_CALLBACK(privmsg)
{
    char *pos_args, *pos_target, str_tags[1024], *str_color, status_msg[2];
    char *color;
    const char *remote_nick, *pv_tags;
    int is_channel, nick_is_me;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;

    IRC_PROTOCOL_MIN_ARGS(4);
    IRC_PROTOCOL_CHECK_HOST;

    if (ignored)
        return WEECHAT_RC_OK;

    pos_args = (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3];

    status_msg[0] = '\0';
    status_msg[1] = '\0';
    pos_target = argv[2];
    is_channel = irc_channel_is_channel (server, pos_target);
    if (!is_channel)
    {
        if (irc_channel_is_channel (server, pos_target + 1)
            && irc_server_prefix_char_statusmsg (server, pos_target[0]))
        {
            is_channel = 1;
            status_msg[0] = pos_target[0];
            pos_target++;
        }
    }

    /* receiver is a channel ? */
    if (is_channel)
    {
        ptr_channel = irc_channel_search (server, pos_target);
        if (ptr_channel)
        {
            /*
             * unmask a smart filtered join if it is in hashtable
             * "join_smart_filtered" of channel
             */
            irc_channel_join_smart_filtered_unmask (ptr_channel, nick);

            /* CTCP to channel */
            if (pos_args[0] == '\01')
            {
                irc_ctcp_recv (server, date, command, ptr_channel,
                               address, nick, NULL, pos_args, argv_eol[0]);
                return WEECHAT_RC_OK;
            }

            /* other message */
            ptr_nick = irc_nick_search (server, ptr_channel, nick);

            if (ptr_nick)
                irc_nick_set_host (ptr_nick, address);

            if (status_msg[0])
            {
                /* message to channel ops/voiced (to "@#channel" or "+#channel") */
                weechat_printf_date_tags (
                    ptr_channel->buffer,
                    date,
                    irc_protocol_tags (command, "notify_message", nick,
                                       address),
                    "%s%s%s%s%s(%s%s%s)%s: %s",
                    weechat_prefix ("network"),
                    "Msg",
                    (status_msg[0]) ? ":" : "",
                    status_msg,
                    IRC_COLOR_CHAT_DELIMITERS,
                    irc_nick_color_for_msg (server, 0, ptr_nick, nick),
                    (nick && nick[0]) ? nick : "?",
                    IRC_COLOR_CHAT_DELIMITERS,
                    IRC_COLOR_RESET,
                    pos_args);
            }
            else
            {
                /* standard message (to "#channel") */
                color = irc_nick_find_color_name ((ptr_nick) ? ptr_nick->name : nick);
                str_color = irc_color_for_tags (color);
                if (color)
                    free (color);
                snprintf (str_tags, sizeof (str_tags),
                          "notify_message,prefix_nick_%s",
                          (str_color) ? str_color : "default");
                if (str_color)
                    free (str_color);
                weechat_printf_date_tags (
                    ptr_channel->buffer,
                    date,
                    irc_protocol_tags (command, str_tags, nick, address),
                    "%s%s",
                    irc_nick_as_prefix (server, ptr_nick,
                                        (ptr_nick) ? NULL : nick,
                                        NULL),
                    pos_args);
            }

            irc_channel_nick_speaking_add (
                ptr_channel,
                nick,
                weechat_string_has_highlight (pos_args,
                                              server->nick));
            irc_channel_nick_speaking_time_remove_old (ptr_channel);
            irc_channel_nick_speaking_time_add (server, ptr_channel, nick,
                                                time (NULL));
        }
    }
    else
    {
        nick_is_me = (irc_server_strcasecmp (server, server->nick, nick) == 0);

        remote_nick = (nick_is_me) ? pos_target : nick;

        /* CTCP to user */
        if (pos_args[0] == '\01')
        {
            irc_ctcp_recv (server, date, command, NULL,
                           address, nick, remote_nick, pos_args, argv_eol[0]);
            return WEECHAT_RC_OK;
        }

        /* private message received => display it */
        ptr_channel = irc_channel_search (server, remote_nick);

        if (!ptr_channel)
        {
            ptr_channel = irc_channel_new (server,
                                           IRC_CHANNEL_TYPE_PRIVATE,
                                           remote_nick, 0, 0);
            if (!ptr_channel)
            {
                weechat_printf (server->buffer,
                                _("%s%s: cannot create new "
                                  "private buffer \"%s\""),
                                weechat_prefix ("error"),
                                IRC_PLUGIN_NAME, remote_nick);
                return WEECHAT_RC_ERROR;
            }
        }
        irc_channel_set_topic (ptr_channel, address);

        if (nick_is_me)
        {
            str_color = irc_color_for_tags (
                weechat_config_color (
                    weechat_config_get ("weechat.color.chat_nick_self")));
        }
        else
        {
            if (weechat_config_boolean (irc_config_look_color_pv_nick_like_channel))
            {
                color = irc_nick_find_color_name (nick);
                str_color = irc_color_for_tags (color);
                if (color)
                    free (color);
            }
            else
            {
                str_color = irc_color_for_tags (
                    weechat_config_color (
                        weechat_config_get ("weechat.color.chat_nick_other")));
            }
        }
        if (nick_is_me)
        {
            snprintf (str_tags, sizeof (str_tags),
                      "self_msg,notify_none,no_highlight,prefix_nick_%s",
                      (str_color) ? str_color : "default");
        }
        else
        {
            pv_tags = weechat_config_string (irc_config_look_pv_tags);
            snprintf (str_tags, sizeof (str_tags),
                      "%s%sprefix_nick_%s",
                      (pv_tags && pv_tags[0]) ? pv_tags : "",
                      (pv_tags && pv_tags[0]) ? "," : "",
                      (str_color) ? str_color : "default");
        }
        if (str_color)
            free (str_color);
        weechat_printf_date_tags (
            ptr_channel->buffer,
            date,
            irc_protocol_tags (command, str_tags, nick, address),
            "%s%s",
            irc_nick_as_prefix (
                server, NULL, nick,
                (nick_is_me) ?
                IRC_COLOR_CHAT_NICK_SELF : irc_nick_color_for_pv (ptr_channel, nick)),
            pos_args);

        if (ptr_channel->has_quit_server)
            ptr_channel->has_quit_server = 0;

        (void) weechat_hook_signal_send ("irc_pv",
                                         WEECHAT_HOOK_SIGNAL_STRING,
                                         argv_eol[0]);
    }

    return WEECHAT_RC_OK;
}