IRC_PROTOCOL_CALLBACK(901)
{
    IRC_PROTOCOL_MIN_ARGS(6);

    if (argc >= 7)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s",
            weechat_prefix ("network"),
            (argv_eol[6][0] == ':') ? argv_eol[6] + 1 : argv_eol[6]);
    }
    else
    {
        irc_protocol_cb_numeric (server,
                                 date, nick, address, host, command,
                                 ignored, argc, argv, argv_eol);
    }

    return WEECHAT_RC_OK;
}