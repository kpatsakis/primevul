IRC_PROTOCOL_CALLBACK(notice)
{
    char *pos_target, *pos_args, *pos, end_char, *channel, status_notice[2];
    const char *nick_address;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    int notify_private, is_channel, is_channel_orig;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(3);

    if (ignored)
        return WEECHAT_RC_OK;

    status_notice[0] = '\0';
    status_notice[1] = '\0';

    if (argv[0][0] == ':')
    {
        if (argc < 4)
            return WEECHAT_RC_ERROR;
        pos_target = argv[2];
        is_channel = irc_channel_is_channel (server, pos_target + 1);
        if (is_channel
            && irc_server_prefix_char_statusmsg (server, pos_target[0]))
        {
            status_notice[0] = pos_target[0];
            pos_target++;
        }
        pos_args = (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3];
        if ((status_notice[0])
            && (pos_args[0] == status_notice[0]) && (pos_args[1] == ' '))
        {
            pos_args += 2;
        }
    }
    else
    {
        pos_target = NULL;
        pos_args = (argv_eol[2][0] == ':') ? argv_eol[2] + 1 : argv_eol[2];
    }

    if (nick && (pos_args[0] == '\01'))
    {
        irc_ctcp_display_reply_from_nick (server, date, command, nick, address,
                                          pos_args);
    }
    else
    {
        is_channel = 0;
        is_channel_orig = 0;
        channel = NULL;
        if (pos_target)
        {
            is_channel = irc_channel_is_channel (server, pos_target);
            is_channel_orig = is_channel;
            if (is_channel)
            {
                channel = strdup (pos_target);
            }
            else if (weechat_config_boolean (irc_config_look_notice_welcome_redirect))
            {
                end_char = ' ';
                switch (pos_args[0])
                {
                    case '[':
                        end_char = ']';
                        break;
                    case '(':
                        end_char = ')';
                        break;
                    case '{':
                        end_char = '}';
                        break;
                    case '<':
                        end_char = '>';
                        break;
                }
                if (end_char != ' ')
                {
                    pos = strchr (pos_args, end_char);
                    if (pos && (pos > pos_args + 1))
                    {
                        channel = weechat_strndup (pos_args + 1,
                                                   pos - pos_args - 1);
                        if (channel && irc_channel_search (server, channel))
                        {
                            is_channel = 1;
                            pos_args = pos + 1;
                            while (pos_args[0] == ' ')
                            {
                                pos_args++;
                            }
                        }
                    }
                }
            }
        }
        if (is_channel)
        {
            /* notice for channel */
            ptr_channel = irc_channel_search (server, channel);

            /*
             * unmask a smart filtered join if it is in hashtable
             * "join_smart_filtered" of channel
             */
            if (ptr_channel)
                irc_channel_join_smart_filtered_unmask (ptr_channel, nick);

            ptr_nick = irc_nick_search (server, ptr_channel, nick);
            weechat_printf_date_tags (
                (ptr_channel) ? ptr_channel->buffer : server->buffer,
                date,
                irc_protocol_tags (command,
                                   (is_channel_orig) ?
                                   "notify_message" :
                                   weechat_config_string (irc_config_look_notice_welcome_tags),
                                   nick, address),
                "%s%s%s%s%s%s%s(%s%s%s)%s: %s",
                weechat_prefix ("network"),
                IRC_COLOR_NOTICE,
                (is_channel_orig) ? "" : "Pv",
                /* TRANSLATORS: "Notice" is command name in IRC protocol (translation is frequently the same word) */
                _("Notice"),
                (status_notice[0]) ? ":" : "",
                status_notice,
                IRC_COLOR_CHAT_DELIMITERS,
                irc_nick_color_for_msg (server, 0, ptr_nick, nick),
                (nick && nick[0]) ? nick : "?",
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_RESET,
                pos_args);
        }
        else
        {
            /* notice for user */
            notify_private = 0;
            if (server->is_connected
                && nick
                && (weechat_strcasecmp (nick, "nickserv") != 0)
                && (weechat_strcasecmp (nick, "chanserv") != 0)
                && (weechat_strcasecmp (nick, "memoserv") != 0))
            {
                /*
                 * add tag "notify_private" only if:
                 *   - server is connected (message 001 already received)
                 * and:
                 *   - notice is from a non-empty nick different from
                 *     nickserv/chanserv/memoserv
                 */
                notify_private = 1;
            }

            ptr_channel = NULL;
            if (nick
                && weechat_config_integer (irc_config_look_notice_as_pv) != IRC_CONFIG_LOOK_NOTICE_AS_PV_NEVER)
            {
                ptr_channel = irc_channel_search (server, nick);
                if (!ptr_channel
                    && weechat_config_integer (irc_config_look_notice_as_pv) == IRC_CONFIG_LOOK_NOTICE_AS_PV_ALWAYS)
                {
                    ptr_channel = irc_channel_new (server,
                                                   IRC_CHANNEL_TYPE_PRIVATE,
                                                   nick, 0, 0);
                    if (!ptr_channel)
                    {
                        weechat_printf (server->buffer,
                                        _("%s%s: cannot create new "
                                          "private buffer \"%s\""),
                                        weechat_prefix ("error"),
                                        IRC_PLUGIN_NAME, nick);
                    }
                }
            }

            if (ptr_channel)
            {
                if (!ptr_channel->topic)
                    irc_channel_set_topic (ptr_channel, address);

                weechat_printf_date_tags (
                    ptr_channel->buffer,
                    date,
                    irc_protocol_tags (command, "notify_private", nick,
                                       address),
                    "%s%s%s%s: %s",
                    weechat_prefix ("network"),
                    irc_nick_color_for_msg (server, 0, NULL, nick),
                    nick,
                    IRC_COLOR_RESET,
                    pos_args);
                if ((ptr_channel->type == IRC_CHANNEL_TYPE_PRIVATE)
                    && ptr_channel->has_quit_server)
                {
                    ptr_channel->has_quit_server = 0;
                }
            }
            else
            {
                ptr_buffer = irc_msgbuffer_get_target_buffer (server, nick,
                                                              command, NULL,
                                                              NULL);
                /*
                 * if notice is sent from myself (for example another WeeChat
                 * via relay), then display message of outgoing notice
                 */
                if (nick && (irc_server_strcasecmp (server, server->nick, nick) == 0))
                {
                    weechat_printf_date_tags (
                        ptr_buffer,
                        date,
                        irc_protocol_tags (command,
                                           (notify_private) ? "notify_private" : NULL,
                                           server->nick, address),
                        "%s%s%s%s -> %s%s%s: %s",
                        weechat_prefix ("network"),
                        IRC_COLOR_NOTICE,
                        /* TRANSLATORS: "Notice" is command name in IRC protocol (translation is frequently the same word) */
                        _("Notice"),
                        IRC_COLOR_RESET,
                        irc_nick_color_for_msg (server, 0, NULL, pos_target),
                        pos_target,
                        IRC_COLOR_RESET,
                        pos_args);
                }
                else
                {
                    nick_address = irc_protocol_nick_address (server, 0, NULL,
                                                              nick, address);
                    weechat_printf_date_tags (
                        ptr_buffer,
                        date,
                        irc_protocol_tags (command,
                                           (notify_private) ? "notify_private" : NULL,
                                           nick, address),
                        "%s%s%s%s",
                        weechat_prefix ("network"),
                        nick_address,
                        (nick_address[0]) ? ": " : "",
                        pos_args);
                }
            }
        }
        if (channel)
            free (channel);
    }

    return WEECHAT_RC_OK;
}