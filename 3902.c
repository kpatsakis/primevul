IRC_PROTOCOL_CALLBACK(366)
{
    struct t_irc_channel *ptr_channel;
    struct t_infolist *infolist;
    struct t_config_option *ptr_option;
    int num_nicks, num_op, num_halfop, num_voice, num_normal, length, i;
    char *string, str_nicks_count[2048], *color;
    const char *prefix, *prefix_color, *nickname;

    IRC_PROTOCOL_MIN_ARGS(5);

    ptr_channel = irc_channel_search (server, argv[3]);
    if (ptr_channel && ptr_channel->nicks)
    {
        /* display users on channel */
        if (weechat_hashtable_has_key (ptr_channel->join_msg_received, "353")
            || weechat_hashtable_has_key (irc_config_hashtable_display_join_message, "353"))
        {
            infolist = weechat_infolist_get ("nicklist", ptr_channel->buffer, NULL);
            if (infolist)
            {
                length = 0;
                while (weechat_infolist_next (infolist))
                {
                    if (strcmp (weechat_infolist_string (infolist, "type"),
                                "nick") == 0)
                    {
                        ptr_option = weechat_config_get (weechat_infolist_string (infolist,
                                                                                  "prefix_color"));
                        length +=
                            ((ptr_option) ? strlen (weechat_color (weechat_config_string (ptr_option))) : 0) +
                            strlen (weechat_infolist_string (infolist, "prefix")) +
                            16 + /* nick color */
                            strlen (weechat_infolist_string (infolist, "name")) +
                            16 + /* reset color */
                            1; /* space */
                    }
                }
                if (length > 0)
                {
                    string = malloc (length);
                    if (string)
                    {
                        string[0] = '\0';
                        i = 0;
                        while (weechat_infolist_next (infolist))
                        {
                            if (strcmp (weechat_infolist_string (infolist, "type"),
                                        "nick") == 0)
                            {
                                if (i > 0)
                                {
                                    strcat (string, IRC_COLOR_RESET);
                                    strcat (string, " ");
                                }
                                prefix = weechat_infolist_string (infolist, "prefix");
                                if (prefix[0] && (prefix[0] != ' '))
                                {
                                    prefix_color = weechat_infolist_string (infolist,
                                                                            "prefix_color");
                                    if (strchr (prefix_color, '.'))
                                    {
                                        ptr_option = weechat_config_get (weechat_infolist_string (infolist,
                                                                                                  "prefix_color"));
                                        if (ptr_option)
                                            strcat (string, weechat_color (weechat_config_string (ptr_option)));
                                    }
                                    else
                                    {
                                        strcat (string, weechat_color (prefix_color));
                                    }
                                    strcat (string, prefix);
                                }
                                nickname = weechat_infolist_string (infolist, "name");
                                if (weechat_config_boolean (irc_config_look_color_nicks_in_names))
                                {
                                    if (irc_server_strcasecmp (server, nickname, server->nick) == 0)
                                        strcat (string, IRC_COLOR_CHAT_NICK_SELF);
                                    else
                                    {
                                        color = irc_nick_find_color (nickname);
                                        strcat (string, color);
                                        if (color)
                                            free (color);
                                    }
                                }
                                else
                                    strcat (string, IRC_COLOR_RESET);
                                strcat (string, nickname);
                                i++;
                            }
                        }
                        weechat_printf_date_tags (
                            irc_msgbuffer_get_target_buffer (
                                server, NULL, command, "names",
                                ptr_channel->buffer),
                            date,
                            irc_protocol_tags (
                                command, "irc_numeric", NULL, NULL),
                            _("%sNicks %s%s%s: %s[%s%s]"),
                            weechat_prefix ("network"),
                            IRC_COLOR_CHAT_CHANNEL,
                            ptr_channel->name,
                            IRC_COLOR_RESET,
                            IRC_COLOR_CHAT_DELIMITERS,
                            string,
                            IRC_COLOR_CHAT_DELIMITERS);
                        free (string);
                    }
                }
                weechat_infolist_free (infolist);
            }
        }

        /* display number of nicks, ops, halfops & voices on the channel */
        if (weechat_hashtable_has_key (ptr_channel->join_msg_received, "366")
            || weechat_hashtable_has_key (irc_config_hashtable_display_join_message, "366"))
        {
            irc_nick_count (server, ptr_channel, &num_nicks, &num_op, &num_halfop,
                            &num_voice, &num_normal);
            str_nicks_count[0] = '\0';
            if (irc_server_get_prefix_mode_index (server, 'o') >= 0)
            {
                length = strlen (str_nicks_count);
                snprintf (str_nicks_count + length,
                          sizeof (str_nicks_count) - length,
                          "%s%s%d%s %s",
                          (str_nicks_count[0]) ? ", " : "",
                          IRC_COLOR_CHAT_CHANNEL,
                          num_op,
                          IRC_COLOR_RESET,
                          NG_("op", "ops", num_op));
            }
            if (irc_server_get_prefix_mode_index (server, 'h') >= 0)
            {
                length = strlen (str_nicks_count);
                snprintf (str_nicks_count + length,
                          sizeof (str_nicks_count) - length,
                          "%s%s%d%s %s",
                          (str_nicks_count[0]) ? ", " : "",
                          IRC_COLOR_CHAT_CHANNEL,
                          num_halfop,
                          IRC_COLOR_RESET,
                          NG_("halfop", "halfops", num_halfop));
            }
            if (irc_server_get_prefix_mode_index (server, 'v') >= 0)
            {
                length = strlen (str_nicks_count);
                snprintf (str_nicks_count + length,
                          sizeof (str_nicks_count) - length,
                          "%s%s%d%s %s",
                          (str_nicks_count[0]) ? ", " : "",
                          IRC_COLOR_CHAT_CHANNEL,
                          num_voice,
                          IRC_COLOR_RESET,
                          NG_("voice", "voices", num_voice));
            }
            length = strlen (str_nicks_count);
            snprintf (str_nicks_count + length,
                      sizeof (str_nicks_count) - length,
                      "%s%s%d%s %s",
                      (str_nicks_count[0]) ? ", " : "",
                      IRC_COLOR_CHAT_CHANNEL,
                      num_normal,
                      IRC_COLOR_RESET,
                      NG_("normal", "normals", num_normal));
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, "names", ptr_channel->buffer),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                _("%sChannel %s%s%s: %s%d%s %s %s(%s%s)"),
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_CHANNEL,
                ptr_channel->name,
                IRC_COLOR_RESET,
                IRC_COLOR_CHAT_CHANNEL,
                num_nicks,
                IRC_COLOR_RESET,
                NG_("nick", "nicks", num_nicks),
                IRC_COLOR_CHAT_DELIMITERS,
                str_nicks_count,
                IRC_COLOR_CHAT_DELIMITERS);
        }

        if (!weechat_hashtable_has_key (ptr_channel->join_msg_received, command))
        {
            irc_command_mode_server (server, "MODE", ptr_channel, NULL,
                                     IRC_SERVER_SEND_OUTQ_PRIO_LOW);
            irc_channel_check_whox (server, ptr_channel);
        }
    }
    else
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "names", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s%s%s: %s",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_RESET,
            (argv[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]);
    }

    if (ptr_channel)
    {
        weechat_hashtable_set (ptr_channel->join_msg_received, "353", "1");
        weechat_hashtable_set (ptr_channel->join_msg_received, "366", "1");
    }

    weechat_bar_item_update ("input_prompt");

    return WEECHAT_RC_OK;
}