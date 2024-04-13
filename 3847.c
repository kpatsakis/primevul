IRC_PROTOCOL_CALLBACK(numeric)
{
    char *pos_args;

    IRC_PROTOCOL_MIN_ARGS(3);

    if (irc_server_strcasecmp (server, server->nick, argv[2]) == 0)
    {
        pos_args = (argc > 3) ?
            ((argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]) : NULL;
    }
    else
    {
        pos_args = (argv_eol[2][0] == ':') ? argv_eol[2] + 1 : argv_eol[2];
    }

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s",
        weechat_prefix ("network"),
        pos_args);

    return WEECHAT_RC_OK;
}