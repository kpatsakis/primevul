IRC_PROTOCOL_CALLBACK(734)
{
    char *pos_args;

    IRC_PROTOCOL_MIN_ARGS(5);

    pos_args = (argc > 5) ?
        ((argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5]) : NULL;

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, NULL, command, "monitor", NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s (%s)",
        weechat_prefix ("error"),
        (pos_args && pos_args[0]) ? pos_args : "",
        argv[3]);

    return WEECHAT_RC_OK;
}