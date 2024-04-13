IRC_PROTOCOL_CALLBACK(chghost)
{
    int length, local_chghost, smart_filter;
    char *str_host, *pos_new_host;
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    struct t_irc_channel_speaking *ptr_nick_speaking;

    IRC_PROTOCOL_MIN_ARGS(4);
    IRC_PROTOCOL_CHECK_HOST;

    local_chghost = (irc_server_strcasecmp (server, nick, server->nick) == 0);

    pos_new_host = (argv_eol[3][0] == ':') ? argv_eol[3] + 1 : argv_eol[3];

    length = strlen (argv[2]) + 1 + strlen (pos_new_host) + 1;
    str_host = malloc (length);
    if (!str_host)
    {
        weechat_printf (
            server->buffer,
            _("%s%s: not enough memory for \"%s\" command"),
            weechat_prefix ("error"), IRC_PLUGIN_NAME, "chghost");
        return WEECHAT_RC_OK;
    }
    snprintf (str_host, length, "%s@%s", argv[2], pos_new_host);

    if (local_chghost)
        irc_server_set_host (server, str_host);

    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        ptr_nick = irc_nick_search (server, ptr_channel, nick);
        if (ptr_nick)
        {
            if (!ignored)
            {
                ptr_nick_speaking = ((weechat_config_boolean (irc_config_look_smart_filter))
                                     && (weechat_config_boolean (irc_config_look_smart_filter_chghost))) ?
                    irc_channel_nick_speaking_time_search (server, ptr_channel, nick, 1) : NULL;
                smart_filter = (!local_chghost
                                && weechat_config_boolean (irc_config_look_smart_filter)
                                && weechat_config_boolean (irc_config_look_smart_filter_chghost)
                                && !ptr_nick_speaking);

                weechat_printf_date_tags (
                    irc_msgbuffer_get_target_buffer (
                        server, NULL, command, NULL, ptr_channel->buffer),
                    date,
                    irc_protocol_tags (
                        command,
                        smart_filter ? "irc_smart_filter" : NULL,
                        nick, address),
                    _("%s%s%s%s (%s%s%s)%s has changed host to %s%s"),
                    weechat_prefix ("network"),
                    irc_nick_color_for_msg (server, 1, ptr_nick, nick),
                    nick,
                    IRC_COLOR_CHAT_DELIMITERS,
                    IRC_COLOR_CHAT_HOST,
                    (address) ? address : "",
                    IRC_COLOR_CHAT_DELIMITERS,
                    IRC_COLOR_MESSAGE_CHGHOST,
                    IRC_COLOR_CHAT_HOST,
                    str_host);
            }
            irc_nick_set_host (ptr_nick, str_host);
        }
    }

    free (str_host);

    return WEECHAT_RC_OK;
}