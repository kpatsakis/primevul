IRC_PROTOCOL_CALLBACK(730)
{
    struct t_irc_notify *ptr_notify;
    const char *monitor_nick, *monitor_host;
    char **nicks;
    int i, num_nicks;

    IRC_PROTOCOL_MIN_ARGS(4);

    nicks = weechat_string_split ((argv_eol[3][0] == ':') ?
                                  argv_eol[3] + 1 : argv_eol[3],
                                  ",",
                                  NULL,
                                  WEECHAT_STRING_SPLIT_STRIP_LEFT
                                  | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                                  | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                                  0,
                                  &num_nicks);
    if (nicks)
    {
        for (i = 0; i < num_nicks; i++)
        {
            monitor_nick = irc_message_get_nick_from_host (nicks[i]);
            monitor_host = strchr (nicks[i], '!');
            if (monitor_host)
                monitor_host++;
            ptr_notify = irc_notify_search (server, monitor_nick);
            if (ptr_notify)
                irc_notify_set_is_on_server (ptr_notify, monitor_host, 1);
        }
        weechat_string_free_split (nicks);
    }

    return WEECHAT_RC_OK;
}