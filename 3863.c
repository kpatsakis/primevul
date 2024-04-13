IRC_PROTOCOL_CALLBACK(321)
{
    char *pos_args;

    IRC_PROTOCOL_MIN_ARGS(4);

    pos_args = (argc > 4) ?
        ((argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4]) : NULL;

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, NULL, command, "list", NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s%s%s",
        weechat_prefix ("network"),
        argv[3],
        (pos_args) ? " " : "",
        (pos_args) ? pos_args : "");

    return WEECHAT_RC_OK;
}