IRC_PROTOCOL_CALLBACK(sasl_end_fail)
{
    int sasl_fail;

    irc_protocol_cb_numeric (server,
                             date, nick, address, host, command,
                             ignored, argc, argv, argv_eol);

    sasl_fail = IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_SASL_FAIL);
    if ((sasl_fail == IRC_SERVER_SASL_FAIL_RECONNECT)
        || (sasl_fail == IRC_SERVER_SASL_FAIL_DISCONNECT))
    {
        irc_server_disconnect (
            server, 0,
            (sasl_fail == IRC_SERVER_SASL_FAIL_RECONNECT) ? 1 : 0);
        return WEECHAT_RC_OK;
    }

    if (!server->is_connected)
        irc_server_sendf (server, 0, NULL, "CAP END");

    return WEECHAT_RC_OK;
}