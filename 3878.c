IRC_PROTOCOL_CALLBACK(732)
{
    char *pos_args;

    IRC_PROTOCOL_MIN_ARGS(3);

    pos_args = (argc > 3) ?
        ((argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]) : NULL;

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, NULL, command, "monitor", NULL),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s",
        weechat_prefix ("network"),
        (pos_args && pos_args[0]) ? pos_args : "");

    return WEECHAT_RC_OK;
}