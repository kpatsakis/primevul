IRC_PROTOCOL_CALLBACK(351)
{
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(5);

    ptr_buffer = irc_msgbuffer_get_target_buffer (server, NULL, command, NULL,
                                                  NULL);

    if (argc > 5)
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s %s (%s)",
            weechat_prefix ("network"),
            argv[3],
            argv[4],
            (argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5]);
    }
    else
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s %s",
            weechat_prefix ("network"),
            argv[3],
            argv[4]);
    }

    return WEECHAT_RC_OK;
}