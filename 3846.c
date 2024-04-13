IRC_PROTOCOL_CALLBACK(kick)
{
    char *pos_comment;
    const char *ptr_autorejoin;
    int rejoin;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick, *ptr_nick_kicked;

    IRC_PROTOCOL_MIN_ARGS(4);
    IRC_PROTOCOL_CHECK_HOST;

    pos_comment = (argc > 4) ?
        ((argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]) : NULL;

    ptr_channel = irc_channel_search (server, argv[2]);
    if (!ptr_channel)
        return WEECHAT_RC_OK;

    ptr_nick = irc_nick_search (server, ptr_channel, nick);
    ptr_nick_kicked = irc_nick_search (server, ptr_channel, argv[3]);

    if (pos_comment)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL,
                                             ptr_channel->buffer),
            date,
            irc_protocol_tags (command, NULL, NULL, address),
            _("%s%s%s%s has kicked %s%s%s %s(%s%s%s)"),
            weechat_prefix ("quit"),
            irc_nick_color_for_msg (server, 1, ptr_nick, nick),
            nick,
            IRC_COLOR_MESSAGE_KICK,
            irc_nick_color_for_msg (server, 1, ptr_nick_kicked, argv[3]),
            argv[3],
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
            _("%s%s%s%s has kicked %s%s%s"),
            weechat_prefix ("quit"),
            irc_nick_color_for_msg (server, 1, ptr_nick, nick),
            nick,
            IRC_COLOR_MESSAGE_KICK,
            irc_nick_color_for_msg (server, 1, ptr_nick_kicked, argv[3]),
            argv[3],
            IRC_COLOR_MESSAGE_KICK);
    }

    if (irc_server_strcasecmp (server, argv[3], server->nick) == 0)
    {
        /*
         * my nick was kicked => free all nicks, channel is not active any
         * more
         */
        irc_nick_free_all (server, ptr_channel);

        irc_channel_modelist_set_state (ptr_channel,
                                        IRC_MODELIST_STATE_MODIFIED);

        /* read option "autorejoin" in server */
        rejoin = IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_AUTOREJOIN);

        /*
         * if buffer has a local variable "autorejoin", use it
         * (it has higher priority than server option
         */
        ptr_autorejoin = weechat_buffer_get_string (ptr_channel->buffer,
                                                    "localvar_autorejoin");
        if (ptr_autorejoin)
            rejoin = weechat_config_string_to_boolean (ptr_autorejoin);

        if (rejoin)
        {
            if (IRC_SERVER_OPTION_INTEGER(server,
                                          IRC_SERVER_OPTION_AUTOREJOIN_DELAY) == 0)
            {
                /* immediately rejoin if delay is 0 */
                irc_channel_rejoin (server, ptr_channel);
            }
            else
            {
                /* rejoin channel later, according to delay */
                ptr_channel->hook_autorejoin =
                    weechat_hook_timer (
                        IRC_SERVER_OPTION_INTEGER(server,
                                                  IRC_SERVER_OPTION_AUTOREJOIN_DELAY) * 1000,
                        0, 1,
                        &irc_channel_autorejoin_cb,
                        ptr_channel, NULL);
            }
        }

        irc_bar_item_update_channel ();
    }
    else
    {
        /*
         * someone was kicked from channel (but not me) => remove only this
         * nick
         */
        if (ptr_nick_kicked)
            irc_nick_free (server, ptr_channel, ptr_nick_kicked);
    }

    return WEECHAT_RC_OK;
}