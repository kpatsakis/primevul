IRC_PROTOCOL_CALLBACK(438)
{
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(4);

    ptr_buffer = irc_msgbuffer_get_target_buffer (server, NULL,
                                                  command, NULL, NULL);

    if (argc >= 5)
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s (%s => %s)",
            weechat_prefix ("network"),
            (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4],
            argv[2],
            argv[3]);
    }
    else
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s %s",
            weechat_prefix ("network"),
            argv[2],
            argv[3]);
    }

    return WEECHAT_RC_OK;
}