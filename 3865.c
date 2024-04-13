IRC_PROTOCOL_CALLBACK(322)
{
    char *pos_topic;

    IRC_PROTOCOL_MIN_ARGS(5);

    pos_topic = (argc > 5) ?
        ((argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5]) : NULL;

    if (!server->cmd_list_regexp ||
        (regexec (server->cmd_list_regexp, argv[3], 0, NULL, 0) == 0))
    {
        weechat_printf_date_tags (
            irc_msgbuffer_get_target_buffer (
                server, NULL, command, "list", NULL),
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            "%s%s%s%s(%s%s%s)%s%s%s",
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_CHANNEL,
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            argv[4],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            (pos_topic && pos_topic[0]) ? ": " : "",
            (pos_topic && pos_topic[0]) ? pos_topic : "");
    }

    return WEECHAT_RC_OK;
}