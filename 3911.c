IRC_PROTOCOL_CALLBACK(nick)
{
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick, *ptr_nick_found;
    char *new_nick, *old_color, str_tags[512];
    const char *buffer_name;
    int local_nick, smart_filter;
    struct t_irc_channel_speaking *ptr_nick_speaking;

    IRC_PROTOCOL_MIN_ARGS(3);
    IRC_PROTOCOL_CHECK_HOST;

    new_nick = (argv[2][0] == ':') ? argv[2] + 1 : argv[2];

    local_nick = (irc_server_strcasecmp (server, nick, server->nick) == 0) ? 1 : 0;

    if (local_nick)
    {
        irc_server_set_nick (server, new_nick);
        irc_server_set_host (server, address);
    }

    ptr_nick_found = NULL;

    /* first display message in server buffer if it's local nick */
    if (local_nick)
    {
        /* temporary disable hotlist */
        weechat_buffer_set (NULL, "hotlist", "-");

        snprintf (str_tags, sizeof (str_tags),
                  "irc_nick1_%s,irc_nick2_%s",
                  nick,
                  new_nick);
        weechat_printf_date_tags (
            server->buffer,
            date,
            irc_protocol_tags (command, str_tags, NULL, address),
            _("%sYou are now known as %s%s%s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_NICK_SELF,
            new_nick,
            IRC_COLOR_RESET);

        /* enable hotlist */
        weechat_buffer_set (NULL, "hotlist", "+");
    }

    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        switch (ptr_channel->type)
        {
            case IRC_CHANNEL_TYPE_PRIVATE:
                /* rename private window if this is with "old nick" */
                if ((irc_server_strcasecmp (server, ptr_channel->name, nick) == 0)
                    && !irc_channel_search (server, new_nick))
                {
                    free (ptr_channel->name);
                    ptr_channel->name = strdup (new_nick);
                    if (ptr_channel->pv_remote_nick_color)
                    {
                        free (ptr_channel->pv_remote_nick_color);
                        ptr_channel->pv_remote_nick_color = NULL;
                    }
                    buffer_name = irc_buffer_build_name (server->name,
                                                         ptr_channel->name);
                    weechat_buffer_set (ptr_channel->buffer,
                                        "name", buffer_name);
                    weechat_buffer_set (ptr_channel->buffer,
                                        "short_name", ptr_channel->name);
                    weechat_buffer_set (ptr_channel->buffer,
                                        "localvar_set_channel",
                                        ptr_channel->name);
                }
                break;
            case IRC_CHANNEL_TYPE_CHANNEL:
                /* rename nick in nicklist if found */
                ptr_nick = irc_nick_search (server, ptr_channel, nick);
                if (ptr_nick)
                {
                    ptr_nick_found = ptr_nick;

                    /* set host in nick if needed */
                    irc_nick_set_host (ptr_nick, address);

                    /* change nick and display message on channel */
                    old_color = strdup (ptr_nick->color);
                    irc_nick_change (server, ptr_channel, ptr_nick, new_nick);
                    if (local_nick)
                    {
                        /* temporary disable hotlist */
                        weechat_buffer_set (NULL, "hotlist", "-");

                        snprintf (str_tags, sizeof (str_tags),
                                  "irc_nick1_%s,irc_nick2_%s",
                                  nick,
                                  new_nick);
                        weechat_printf_date_tags (ptr_channel->buffer,
                                                  date,
                                                  irc_protocol_tags (command,
                                                                     str_tags,
                                                                     NULL,
                                                                     address),
                                                  _("%sYou are now known as "
                                                    "%s%s%s"),
                                                  weechat_prefix ("network"),
                                                  IRC_COLOR_CHAT_NICK_SELF,
                                                  new_nick,
                                                  IRC_COLOR_RESET);

                        /* enable hotlist */
                        weechat_buffer_set (NULL, "hotlist", "+");
                    }
                    else
                    {
                        if (!irc_ignore_check (server, ptr_channel->name,
                                               nick, host))
                        {
                            ptr_nick_speaking = ((weechat_config_boolean (irc_config_look_smart_filter))
                                                 && (weechat_config_boolean (irc_config_look_smart_filter_nick))) ?
                                irc_channel_nick_speaking_time_search (server, ptr_channel, nick, 1) : NULL;
                            smart_filter = (weechat_config_boolean (irc_config_look_smart_filter)
                                            && weechat_config_boolean (irc_config_look_smart_filter_nick)
                                            && !ptr_nick_speaking);
                            snprintf (str_tags, sizeof (str_tags),
                                      "%sirc_nick1_%s,irc_nick2_%s",
                                      (smart_filter) ? "irc_smart_filter," : "",
                                      nick,
                                      new_nick);
                            weechat_printf_date_tags (
                                ptr_channel->buffer,
                                date,
                                irc_protocol_tags (command, str_tags, NULL,
                                                   address),
                                _("%s%s%s%s is now known as %s%s%s"),
                                weechat_prefix ("network"),
                                weechat_config_boolean (irc_config_look_color_nicks_in_server_messages) ?
                                old_color : IRC_COLOR_CHAT_NICK,
                                nick,
                                IRC_COLOR_RESET,
                                irc_nick_color_for_msg (server, 1, ptr_nick,
                                                        new_nick),
                                new_nick,
                                IRC_COLOR_RESET);
                        }
                        irc_channel_nick_speaking_rename (ptr_channel,
                                                          nick, new_nick);
                        irc_channel_nick_speaking_time_rename (server,
                                                               ptr_channel,
                                                               nick, new_nick);
                        irc_channel_join_smart_filtered_rename (ptr_channel,
                                                                nick,
                                                                new_nick);
                    }

                    if (old_color)
                        free (old_color);
                }
                break;
        }
    }

    if (!local_nick)
    {
        irc_channel_display_nick_back_in_pv (server, ptr_nick_found, new_nick);
        irc_channel_set_topic_private_buffers (server, ptr_nick_found,
                                               new_nick, address);
    }

    return WEECHAT_RC_OK;
}