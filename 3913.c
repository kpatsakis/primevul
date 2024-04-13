IRC_PROTOCOL_CALLBACK(server_mode_reason)
{
    char *pos_mode, *pos_args;

    IRC_PROTOCOL_MIN_ARGS(3);

    /* skip nickname if at beginning of server message */
    if (irc_server_strcasecmp (server, server->nick, argv[2]) == 0)
    {
        pos_mode = argv[3];
        pos_args = (argc > 4) ?
            ((argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]) : NULL;
    }
    else
    {
        pos_mode = argv[2];
        pos_args = (argc > 3) ?
            ((argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]) : NULL;
    }

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s: %s",
        weechat_prefix ("network"),
        pos_mode,
        (pos_args) ? pos_args : "");

    return WEECHAT_RC_OK;
}