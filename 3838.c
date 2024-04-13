IRC_PROTOCOL_CALLBACK(005)
{
    char *pos, *pos2, *pos_start, *error, *isupport2;
    int length_isupport, length, casemapping;
    long value;

    IRC_PROTOCOL_MIN_ARGS(4);

    irc_protocol_cb_numeric (server,
                             date, nick, address, host, command,
                             ignored, argc, argv, argv_eol);

    /* save prefix */
    pos = strstr (argv_eol[3], "PREFIX=");
    if (pos)
    {
        pos += 7;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        irc_server_set_prefix_modes_chars (server, pos);
        if (pos2)
            pos2[0] = ' ';
    }

    /* save max nick length */
    pos = strstr (argv_eol[3], "NICKLEN=");
    if (pos)
    {
        pos += 8;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        error = NULL;
        value = strtol (pos, &error, 10);
        if (error && !error[0] && (value > 0))
            server->nick_max_length = (int)value;
        if (pos2)
            pos2[0] = ' ';
    }

    /* save max user length */
    pos = strstr (argv_eol[3], "USERLEN=");
    if (pos)
    {
        pos += 8;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        error = NULL;
        value = strtol (pos, &error, 10);
        if (error && !error[0] && (value > 0))
            server->user_max_length = (int)value;
        if (pos2)
            pos2[0] = ' ';
    }

    /* save max host length */
    pos = strstr (argv_eol[3], "HOSTLEN=");
    if (pos)
    {
        pos += 8;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        error = NULL;
        value = strtol (pos, &error, 10);
        if (error && !error[0] && (value > 0))
            server->host_max_length = (int)value;
        if (pos2)
            pos2[0] = ' ';
    }

    /* save casemapping */
    pos = strstr (argv_eol[3], "CASEMAPPING=");
    if (pos)
    {
        pos += 12;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        casemapping = irc_server_search_casemapping (pos);
        if (casemapping >= 0)
            server->casemapping = casemapping;
        if (pos2)
            pos2[0] = ' ';
    }

    /* save chantypes */
    pos = strstr (argv_eol[3], "CHANTYPES=");
    if (pos)
    {
        pos += 10;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        if (server->chantypes)
            free (server->chantypes);
        server->chantypes = strdup (pos);
        if (pos2)
            pos2[0] = ' ';
    }

    /* save chanmodes */
    pos = strstr (argv_eol[3], "CHANMODES=");
    if (pos)
    {
        pos += 10;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        if (server->chanmodes)
            free (server->chanmodes);
        server->chanmodes = strdup (pos);
        if (pos2)
            pos2[0] = ' ';
    }

    /* save monitor (limit) */
    pos = strstr (argv_eol[3], "MONITOR=");
    if (pos)
    {
        pos += 8;
        pos2 = strchr (pos, ' ');
        if (pos2)
            pos2[0] = '\0';
        error = NULL;
        value = strtol (pos, &error, 10);
        if (error && !error[0] && (value > 0))
            server->monitor = (int)value;
        if (pos2)
            pos2[0] = ' ';
    }

    /* save whole message (concatenate to existing isupport, if any) */
    pos_start = NULL;
    pos = strstr (argv_eol[3], " :");
    length = (pos) ? pos - argv_eol[3] : (int)strlen (argv_eol[3]);
    if (server->isupport)
    {
        length_isupport = strlen (server->isupport);
        isupport2 = realloc (server->isupport,
                             length_isupport + /* existing */
                             1 + length + 1); /* new */
        if (isupport2)
        {
            server->isupport = isupport2;
            pos_start = server->isupport + length_isupport;
        }
    }
    else
    {
        server->isupport = malloc (1 + length + 1);
        if (server->isupport)
            pos_start = server->isupport;
    }

    if (pos_start)
    {
        pos_start[0] = ' ';
        memcpy (pos_start + 1, argv_eol[3], length);
        pos_start[length + 1] = '\0';
    }

    return WEECHAT_RC_OK;
}