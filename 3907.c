IRC_PROTOCOL_CALLBACK(001)
{
    char *server_command, **commands, **ptr_command, *command2, *slash_command;
    char *away_msg, *usermode;
    int length;

    IRC_PROTOCOL_MIN_ARGS(3);

    if (irc_server_strcasecmp (server, server->nick, argv[2]) != 0)
        irc_server_set_nick (server, argv[2]);

    irc_protocol_cb_numeric (server,
                             date, nick, address, host, command,
                             ignored, argc, argv, argv_eol);

    /* connection to IRC server is OK! */
    server->is_connected = 1;
    server->reconnect_delay = 0;
    server->monitor_time = time (NULL) + 5;

    if (server->hook_timer_connection)
    {
        weechat_unhook (server->hook_timer_connection);
        server->hook_timer_connection = NULL;
    }
    server->lag_next_check = time (NULL) +
        weechat_config_integer (irc_config_network_lag_check);
    irc_server_set_buffer_title (server);

    /* set away message if user was away (before disconnection for example) */
    if (server->away_message && server->away_message[0])
    {
        away_msg = strdup (server->away_message);
        if (away_msg)
        {
            irc_command_away_server (server, away_msg, 0);
            free (away_msg);
        }
    }

    /* send signal "irc_server_connected" with server name */
    (void) weechat_hook_signal_send ("irc_server_connected",
                                     WEECHAT_HOOK_SIGNAL_STRING, server->name);

    /* set usermode when connected */
    usermode = irc_server_eval_expression (
        server,
        IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_USERMODE));
    if (usermode && usermode[0])
    {
        irc_server_sendf (server,
                          IRC_SERVER_SEND_OUTQ_PRIO_HIGH, NULL,
                          "MODE %s %s",
                          server->nick, usermode);
    }
    if (usermode)
        free (usermode);

    /* execute command when connected */
    server_command = irc_server_eval_expression (
        server,
        IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_COMMAND));
    if (server_command && server_command[0])
    {
        /* split command on ';' which can be escaped with '\;' */
        commands = weechat_string_split_command (server_command, ';');
        if (commands)
        {
            for (ptr_command = commands; *ptr_command; ptr_command++)
            {
                command2 = irc_message_replace_vars (server, NULL,
                                                     *ptr_command);
                if (command2)
                {
                    if (weechat_string_is_command_char (command2))
                    {
                        weechat_command (server->buffer, command2);
                    }
                    else
                    {
                        length = 1 + strlen(command2) + 1;
                        slash_command = malloc (length);
                        if (slash_command)
                        {
                            snprintf (slash_command, length, "/%s", command2);
                            weechat_command (server->buffer, slash_command);
                            free (slash_command);
                        }
                    }
                    free (command2);
                }
            }
            weechat_string_free_split_command (commands);
        }

        if (IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_COMMAND_DELAY) > 0)
            server->command_time = time (NULL) + 1;
        else
            irc_server_autojoin_channels (server);
    }
    else
    {
        irc_server_autojoin_channels (server);
    }
    if (server_command)
        free (server_command);

    return WEECHAT_RC_OK;
}