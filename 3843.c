IRC_PROTOCOL_CALLBACK(account)
{
    struct t_irc_channel *ptr_channel;
    struct t_irc_nick *ptr_nick;
    char *pos_account;
    int cap_account_notify;

    IRC_PROTOCOL_MIN_ARGS(3);

    pos_account = (strcmp (argv[2], "*") != 0) ? argv[2] : NULL;

    cap_account_notify = weechat_hashtable_has_key (server->cap_list,
                                                    "account-notify");

    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        ptr_nick = irc_nick_search (server, ptr_channel, nick);
        if (ptr_nick)
        {
            if (ptr_nick->account)
                free (ptr_nick->account);
            ptr_nick->account = (cap_account_notify && pos_account) ?
                strdup (pos_account) : NULL;
        }
    }

    return WEECHAT_RC_OK;
}