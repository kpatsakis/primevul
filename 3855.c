IRC_PROTOCOL_CALLBACK(432)
{
    const char *alternate_nick;
    struct t_gui_buffer *ptr_buffer;

    irc_protocol_cb_generic_error (server,
                                   date, nick, address, host, command,
                                   ignored, argc, argv, argv_eol);

    if (!server->is_connected)
    {
        ptr_buffer = irc_msgbuffer_get_target_buffer (server, NULL,
                                                      command, NULL, NULL);

        alternate_nick = irc_server_get_alternate_nick (server);
        if (!alternate_nick)
        {
            weechat_printf_date_tags (
                ptr_buffer, date, NULL,
                _("%s%s: all declared nicknames are already in use or "
                  "invalid, closing connection with server"),
                weechat_prefix ("error"), IRC_PLUGIN_NAME);
            irc_server_disconnect (server, 0, 1);
            return WEECHAT_RC_OK;
        }

        weechat_printf_date_tags (
            ptr_buffer, date, NULL,
            _("%s%s: nickname \"%s\" is invalid, trying nickname \"%s\""),
            weechat_prefix ("error"), IRC_PLUGIN_NAME,
            server->nick, alternate_nick);

        irc_server_set_nick (server, alternate_nick);

        irc_server_sendf (
            server, 0, NULL,
            "NICK %s%s",
            (server->nick && strchr (server->nick, ':')) ? ":" : "",
            server->nick);
    }

    return WEECHAT_RC_OK;
}