IRC_PROTOCOL_CALLBACK(305)
{
    IRC_PROTOCOL_MIN_ARGS(3);

    if (argc > 3)
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "unaway", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s",
            weechat_prefix ("network"),
            (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3]);
    }

    server->is_away = 0;
    server->away_time = 0;

    weechat_bar_item_update ("away");

    return WEECHAT_RC_OK;
}