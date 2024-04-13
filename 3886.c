IRC_PROTOCOL_CALLBACK(wallops)
{
    const char *nick_address;

    IRC_PROTOCOL_MIN_ARGS(3);

    if (ignored)
        return WEECHAT_RC_OK;

    nick_address = irc_protocol_nick_address (server, 0, NULL, nick, address);
    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, nick, command, NULL, NULL),
        date,
        irc_protocol_tags (command, NULL, nick, address),
        _("%sWallops from %s: %s"),
        weechat_prefix ("network"),
        (nick_address[0]) ? nick_address : "?",
        (argv_eol[2][0] == ':') ? argv_eol[2] + 1 : argv_eol[2]);

    return WEECHAT_RC_OK;
}