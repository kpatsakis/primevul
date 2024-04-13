IRC_PROTOCOL_CALLBACK(error)
{
    char *ptr_args;

    IRC_PROTOCOL_MIN_ARGS(2);

    ptr_args = (argv_eol[1][0] == ':') ? argv_eol[1] + 1 : argv_eol[1];

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
        date,
        irc_protocol_tags (command, NULL, NULL, NULL),
        "%s%s",
        weechat_prefix ("error"),
        ptr_args);

    if (strncmp (ptr_args, "Closing Link", 12) == 0)
    {
        irc_server_disconnect (server, !server->is_connected, 1);
    }

    return WEECHAT_RC_OK;
}