IRC_PROTOCOL_CALLBACK(sasl_end_ok)
{
    irc_protocol_cb_numeric (server,
                             date, nick, address, host, command,
                             ignored, argc, argv, argv_eol);

    if (!server->is_connected)
        irc_server_sendf (server, 0, NULL, "CAP END");

    return WEECHAT_RC_OK;
}