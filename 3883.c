IRC_PROTOCOL_CALLBACK(join)
{
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    struct t_irc_channel_speaking *ptr_nick_speaking;
    char *pos_channel, *pos_account, *pos_realname;
    char str_account[512], str_realname[512];
    int local_join, display_host, smart_filter;

    IRC_PROTOCOL_MIN_ARGS(3);
    IRC_PROTOCOL_CHECK_HOST;

    local_join = (irc_server_strcasecmp (server, nick, server->nick) == 0);

    pos_channel = (argv[2][0] == ':') ? argv[2] + 1 : argv[2];
    pos_account = ((argc > 3) && (strcmp (argv[3], "*") != 0)) ?
        argv[3] : NULL;
    pos_realname = (argc > 4) ?
        ((argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]) : NULL;

    str_account[0] = '\0';
    if (pos_account)
    {
        snprintf (str_account, sizeof (str_account),
                  "%s [%s%s%s]",
                  IRC_COLOR_CHAT_DELIMITERS,
                  IRC_COLOR_CHAT_HOST,
                  pos_account,
                  IRC_COLOR_CHAT_DELIMITERS);
    }

    str_realname[0] = '\0';
    if (pos_realname)
    {
        snprintf (str_realname, sizeof (str_realname),
                  "%s (%s%s%s)",
                  IRC_COLOR_CHAT_DELIMITERS,
                  IRC_COLOR_CHAT_HOST,
                  pos_realname,
                  IRC_COLOR_CHAT_DELIMITERS);
    }

    ptr_channel = irc_channel_search (server, pos_channel);
    if (ptr_channel)
    {
        ptr_channel->part = 0;
    }
    else
    {
        /*
         * if someone else joins and channel is not opened, then just
         * ignore it (we should receive our self join first)
         */
        if (!local_join)
            return WEECHAT_RC_OK;

        ptr_channel = irc_channel_new (server, IRC_CHANNEL_TYPE_CHANNEL,
                                       pos_channel, 1, 1);
        if (!ptr_channel)
        {
            weechat_printf (server->buffer,
                            _("%s%s: cannot create new channel \"%s\""),
                            weechat_prefix ("error"), IRC_PLUGIN_NAME,
                            pos_channel);
            return WEECHAT_RC_ERROR;
        }
    }

    /*
     * local join? clear nicklist to be sure it is empty (when using znc, after
     * reconnection to network, we receive a JOIN for channel with existing
     * nicks in irc plugin, so we need to clear the nicklist now)
     */
    if (local_join)
        irc_nick_free_all (server, ptr_channel);

    /* reset some variables if joining new channel */
    if (!ptr_channel->nicks)
    {
        irc_channel_set_topic (ptr_channel, NULL);
        if (ptr_channel->modes)
        {
            free (ptr_channel->modes);
            ptr_channel->modes = NULL;
        }
        ptr_channel->limit = 0;
        weechat_hashtable_remove_all (ptr_channel->join_msg_received);
        ptr_channel->checking_whox = 0;
    }

    /* add nick in channel */
    ptr_nick = irc_nick_new (server, ptr_channel, nick, address, NULL, 0,
                             (pos_account) ? pos_account : NULL,
                             (pos_realname) ? pos_realname : NULL);

    /* rename the nick if it was in list with a different case */
    irc_channel_nick_speaking_rename_if_present (server, ptr_channel, nick);

    if (!ignored)
    {
        ptr_nick_speaking = ((weechat_config_boolean (irc_config_look_smart_filter))
                             && (weechat_config_boolean (irc_config_look_smart_filter_join))) ?
            irc_channel_nick_speaking_time_search (server, ptr_channel, nick, 1) : NULL;
        display_host = (local_join) ?
            weechat_config_boolean (irc_config_look_display_host_join_local) :
            weechat_config_boolean (irc_config_look_display_host_join);

        /*
         * "smart" filter the join message is it's not a join from myself, if
         * smart filtering is enabled, and if nick was not speaking in channel
         */
        smart_filter = (!local_join
                        && weechat_config_boolean (irc_config_look_smart_filter)
                        && weechat_config_boolean (irc_config_look_smart_filter_join)
                        && !ptr_nick_speaking);

        /* display the join */
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL,
                                             ptr_channel->buffer),
            date,
            irc_protocol_tags (command,
                               smart_filter ? "irc_smart_filter" : NULL,
                               nick, address),
            _("%s%s%s%s%s%s%s%s%s%s%s%s has joined %s%s%s"),
            weechat_prefix ("join"),
            irc_nick_color_for_msg (server, 1, ptr_nick, nick),
            nick,
            str_account,
            str_realname,
            IRC_COLOR_CHAT_DELIMITERS,
            (display_host) ? " (" : "",
            IRC_COLOR_CHAT_HOST,
            (display_host) ? address : "",
            IRC_COLOR_CHAT_DELIMITERS,
            (display_host) ? ")" : "",
            IRC_COLOR_MESSAGE_JOIN,
            IRC_COLOR_CHAT_CHANNEL,
            pos_channel,
            IRC_COLOR_MESSAGE_JOIN);

        /*
         * if join is smart filtered, save the nick in hashtable, and if nick
         * is speaking shortly after the join, it will be unmasked
         * (option irc.look.smart_filter_join_unmask)
         */
        if (smart_filter)
        {
            irc_channel_join_smart_filtered_add (ptr_channel, nick,
                                                 time (NULL));
        }

        /* display message in private if private has flag "has_quit_server" */
        if (!local_join)
        {
            irc_channel_display_nick_back_in_pv (server, ptr_nick, nick);
            irc_channel_set_topic_private_buffers (server, ptr_nick, nick,
                                                   address);
        }
    }

    if (local_join)
    {
        irc_server_set_host (server, address);
        irc_bar_item_update_channel ();
    }

    return WEECHAT_RC_OK;
}