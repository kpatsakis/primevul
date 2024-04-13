IRC_PROTOCOL_CALLBACK(353)
{
    char *pos_channel, *pos_nick, *pos_nick_orig, *pos_host, *nickname;
    char *prefixes, *str_nicks, *color;
    int args, i, length;
    struct t_irc_channel *ptr_channel;

    IRC_PROTOCOL_MIN_ARGS(5);

    if (irc_channel_is_channel (server, argv[3]))
    {
        pos_channel = argv[3];
        args = 4;
    }
    else
    {
        pos_channel = argv[4];
        args = 5;
    }

    IRC_PROTOCOL_MIN_ARGS(args + 1);

    ptr_channel = irc_channel_search (server, pos_channel);
    str_nicks = NULL;

    /*
     * for a channel without buffer, prepare a string that will be built
     * with nicks and colors (argc - args is the number of nicks)
     */
    if (!ptr_channel)
    {
        /*
         * prefix color (16) + nick color (16) + reset color (16) = 48 bytes
         * added for each nick
         */
        length = strlen (argv_eol[args]) + ((argc - args) * (16 + 16 + 16)) + 1;
        str_nicks = malloc (length);
        if (str_nicks)
            str_nicks[0] = '\0';
    }

    for (i = args; i < argc; i++)
    {
        pos_nick = (argv[i][0] == ':') ? argv[i] + 1 : argv[i];
        pos_nick_orig = pos_nick;

        /* skip and save prefix(es) */
        while (pos_nick[0]
               && (irc_server_get_prefix_char_index (server, pos_nick[0]) >= 0))
        {
            pos_nick++;
        }
        prefixes = (pos_nick > pos_nick_orig) ?
            weechat_strndup (pos_nick_orig, pos_nick - pos_nick_orig) : NULL;

        /* extract nick from host */
        pos_host = strchr (pos_nick, '!');
        if (pos_host)
        {
            nickname = weechat_strndup (pos_nick, pos_host - pos_nick);
            pos_host++;
        }
        else
            nickname = strdup (pos_nick);

        /* add or update nick on channel */
        if (nickname)
        {
            if (ptr_channel && ptr_channel->nicks)
            {
                if (!irc_nick_new (server, ptr_channel, nickname, pos_host,
                                   prefixes, 0, NULL, NULL))
                {
                    weechat_printf (
                        server->buffer,
                        _("%s%s: cannot create nick \"%s\" for channel \"%s\""),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME, nickname,
                        ptr_channel->name);
                }
            }
            else if (!ptr_channel && str_nicks)
            {
                if (str_nicks[0])
                {
                    strcat (str_nicks, IRC_COLOR_RESET);
                    strcat (str_nicks, " ");
                }
                if (prefixes)
                {
                    strcat (str_nicks,
                            weechat_color (
                                irc_nick_get_prefix_color_name (server,
                                                                prefixes[0])));
                    strcat (str_nicks, prefixes);
                }
                if (weechat_config_boolean (irc_config_look_color_nicks_in_names))
                {
                    if (irc_server_strcasecmp (server, nickname, server->nick) == 0)
                        strcat (str_nicks, IRC_COLOR_CHAT_NICK_SELF);
                    else
                    {
                        color = irc_nick_find_color (nickname);
                        strcat (str_nicks, color);
                        if (color)
                            free (color);
                    }
                }
                else
                    strcat (str_nicks, IRC_COLOR_RESET);
                strcat (str_nicks, nickname);
            }
            free (nickname);
        }
        if (prefixes)
            free (prefixes);
    }

    if (!ptr_channel)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "names", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            _("%sNicks %s%s%s: %s[%s%s%s]"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            pos_channel,
            IRC_COLOR_RESET,
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (str_nicks) ? str_nicks : "",
            IRC_COLOR_CHAT_DELIMITERS);
    }

    if (str_nicks)
        free (str_nicks);

    return WEECHAT_RC_OK;
}