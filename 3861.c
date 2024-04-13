irc_protocol_log_level_for_command (const char *command)
{
    if (!command || !command[0])
        return 0;

    if ((strcmp (command, "privmsg") == 0)
        || (strcmp (command, "notice") == 0))
        return 1;

    if (strcmp (command, "nick") == 0)
        return 2;

    if ((strcmp (command, "join") == 0)
        || (strcmp (command, "part") == 0)
        || (strcmp (command, "quit") == 0)
        || (strcmp (command, "nick_back") == 0))
        return 4;

    return 3;
}