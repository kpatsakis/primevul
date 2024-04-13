IRC_PROTOCOL_CALLBACK(generic_error)
{
    int first_arg;
    char *chan_nick, *args;
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(4);

    first_arg = (irc_server_strcasecmp (server, argv[2], server->nick) == 0) ? 3 : 2;

    if ((argv[first_arg][0] != ':') && argv[first_arg + 1])
    {
        chan_nick = argv[first_arg];
        args = argv_eol[first_arg + 1];
    }
    else
    {
        chan_nick = NULL;
        args = argv_eol[first_arg];
    }
    if (args[0] == ':')
        args++;

    ptr_channel = NULL;
    if (chan_nick)
        ptr_channel = irc_channel_search (server, chan_nick);

    ptr_buffer = (ptr_channel) ? ptr_channel->buffer : server->buffer;

    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, NULL, command,
            ((strcmp (command, "401") == 0)
             || (strcmp (command, "402") == 0)) ? "whois" : NULL,
            ptr_buffer),
        date,
        irc_protocol_tags (command, NULL, NULL, NULL),
        "%s%s%s%s%s%s",
        weechat_prefix ("network"),
        (ptr_channel && chan_nick
         && (irc_server_strcasecmp (server, chan_nick,
                                    ptr_channel->name) == 0)) ?
        IRC_COLOR_CHAT_CHANNEL : "",
        (chan_nick) ? chan_nick : "",
        IRC_COLOR_RESET,
        (chan_nick) ? ": " : "",
        args);

    return WEECHAT_RC_OK;
}