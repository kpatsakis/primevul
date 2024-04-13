IRC_PROTOCOL_CALLBACK(away)
{
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;

    IRC_PROTOCOL_MIN_ARGS(2);

    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        ptr_nick = irc_nick_search (server, ptr_channel, nick);
        if (ptr_nick)
            irc_nick_set_away (server, ptr_channel, ptr_nick, (argc > 2));
    }

    return WEECHAT_RC_OK;
}