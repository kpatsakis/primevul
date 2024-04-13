IRC_PROTOCOL_CALLBACK(354)
{
    char *pos_attr, *pos_hopcount, *pos_account, *pos_realname, *str_host;
    int length;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;

    IRC_PROTOCOL_MIN_ARGS(4);

    ptr_channel = irc_channel_search (server, argv[3]);

    /*
     * if there are less than 11 arguments, we are unable to parse the message,
     * some infos are missing but we don't know which ones; in this case we
     * just display the message as-is
     */
    if (argc < 11)
    {
        if (!ptr_channel || (ptr_channel->checking_whox <= 0))
        {
            weechat_printf_date_tags (
                irc_msgbuffer_get_target_buffer (
                    server, NULL, command, "who", NULL),
                date,
                irc_protocol_tags (command, "irc_numeric", NULL, NULL),
                "%s%s[%s%s%s]%s%s%s",
                weechat_prefix ("network"),
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_CHAT_CHANNEL,
                argv[3],
                IRC_COLOR_CHAT_DELIMITERS,
                IRC_COLOR_RESET,
                (argc > 4) ? " " : "",
                (argc > 4) ? argv_eol[4] : "");
        }
        return WEECHAT_RC_OK;
    }

    ptr_nick = (ptr_channel) ?
        irc_nick_search (server, ptr_channel, argv[7]) : NULL;
    pos_attr = argv[8];
    pos_hopcount = argv[9];
    pos_account = (strcmp (argv[10], "0") != 0) ? argv[10] : NULL;
    pos_realname = (argc > 11) ?
        ((argv_eol[11][0] == ':') ? argv_eol[11] + 1 : argv_eol[11]) : NULL;

    /* update host in nick */
    if (ptr_nick)
    {
        length = strlen (argv[4]) + 1 + strlen (argv[5]) + 1;
        str_host = malloc (length);
        if (str_host)
        {
            snprintf (str_host, length, "%s@%s", argv[4], argv[5]);
            irc_nick_set_host (ptr_nick, str_host);
            free (str_host);
        }
    }

    /* update away flag in nick */
    if (ptr_channel && ptr_nick)
    {
        irc_nick_set_away (server, ptr_channel, ptr_nick,
                           (pos_attr && (pos_attr[0] == 'G')) ? 1 : 0);
    }

    /* update account flag in nick */
    if (ptr_nick)
    {
        if (ptr_nick->account)
            free (ptr_nick->account);
        if (ptr_channel && pos_account
            && weechat_hashtable_has_key (server->cap_list, "account-notify"))
        {
            ptr_nick->account = strdup (pos_account);
        }
        else
        {
            ptr_nick->account = NULL;
        }
    }

    /* update realname in nick */
    if (ptr_nick)
    {
        if (ptr_nick->realname)
            free (ptr_nick->realname);
        if (ptr_channel && pos_realname
            && weechat_hashtable_has_key (server->cap_list, "extended-join"))
        {
            ptr_nick->realname = strdup (pos_realname);
        }
        else
        {
            ptr_nick->realname = NULL;
        }
    }

    /* display output of who (manual who from user) */
    if (!ptr_channel || (ptr_channel->checking_whox <= 0))
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "who", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s[%s%s%s] %s%s %s%s%s%s%s%s(%s%s@%s%s)%s %s%s%s%s(%s)",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[7]),
            argv[7],
            IRC_COLOR_CHAT_DELIMITERS,
            (pos_account) ? "[" : "",
            (pos_account) ? IRC_COLOR_CHAT_HOST : "",
            (pos_account) ? pos_account : "",
            (pos_account) ? IRC_COLOR_CHAT_DELIMITERS : "",
            (pos_account) ? "] " : "",
            IRC_COLOR_CHAT_HOST,
            argv[4],
            argv[5],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (pos_attr) ? pos_attr : "",
            (pos_attr) ? " " : "",
            (pos_hopcount) ? pos_hopcount : "",
            (pos_hopcount) ? " " : "",
            (pos_realname) ? pos_realname : "");
    }

    return WEECHAT_RC_OK;
}