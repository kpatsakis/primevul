IRC_PROTOCOL_CALLBACK(invite)
{
    IRC_PROTOCOL_MIN_ARGS(4);
    IRC_PROTOCOL_CHECK_HOST;

    if (ignored)
        return WEECHAT_RC_OK;

    if (irc_server_strcasecmp (server, argv[2], server->nick) == 0)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, nick, command, NULL, NULL),
            date,
            irc_protocol_tags (command, "notify_highlight", nick, address),
            _("%sYou have been invited to %s%s%s by %s%s%s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            (argv[3][0] == ':') ? argv[3] + 1 : argv[3],
            IRC_COLOR_RESET,
            irc_nick_color_for_msg (server, 1, NULL, nick),
            nick,
            IRC_COLOR_RESET);
    }
    else
    {
        /* CAP invite-notify */
        /* imitate numeric 341 output */
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, nick, command, NULL, NULL),
            date,
            irc_protocol_tags (command, NULL, nick, address),
            _("%s%s%s%s has invited %s%s%s to %s%s%s"),
            weechat_prefix ("network"),
            irc_nick_color_for_msg (server, 1, NULL, nick),
            nick,
            IRC_COLOR_RESET,
            irc_nick_color_for_msg (server, 1, NULL, argv[2]),
            argv[2],
            IRC_COLOR_RESET,
            IRC_COLOR_CHAT_CHANNEL,
            (argv[3][0] == ':') ? argv[3] + 1 : argv[3],
            IRC_COLOR_RESET);
    }

    return WEECHAT_RC_OK;
}