IRC_PROTOCOL_CALLBACK(ping)
{
    IRC_PROTOCOL_MIN_ARGS(2);

    irc_server_sendf (server, 0, NULL, "PONG :%s",
                      (argv_eol[1][0] == ':') ? argv_eol[1] + 1 : argv_eol[1]);

    return WEECHAT_RC_OK;
}