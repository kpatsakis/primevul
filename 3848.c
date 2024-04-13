IRC_PROTOCOL_CALLBACK(pong)
{
    struct timeval tv;

    IRC_PROTOCOL_MIN_ARGS(0);

    if (server->lag_check_time.tv_sec != 0)
    {
        /* calculate lag (time diff with lag check) */
        gettimeofday (&tv, NULL);
        server->lag = (int)(weechat_util_timeval_diff (&(server->lag_check_time),
                                                       &tv) / 1000);

        /* schedule next lag check */
        server->lag_check_time.tv_sec = 0;
        server->lag_check_time.tv_usec = 0;
        server->lag_next_check = time (NULL) +
            weechat_config_integer (irc_config_network_lag_check);

        /* refresh lag bar item if needed */
        if (server->lag != server->lag_displayed)
        {
            server->lag_displayed = server->lag;
            irc_server_set_lag (server);
        }
    }
    else
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (server, NULL, command, NULL, NULL),
            date,
            irc_protocol_tags (command, NULL, NULL, NULL),
            "PONG%s%s",
            (argc >= 4) ? ": " : "",
            (argc >= 4) ? ((argv_eol[3][0] == ':') ?
                           argv_eol[3] + 1 : argv_eol[3]) : "");
    }

    return WEECHAT_RC_OK;
}