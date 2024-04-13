irc_protocol_tags (const char *command, const char *tags, const char *nick,
                   const char *address)
{
    static char string[1024];
    int log_level;
    char str_log_level[32];

    str_log_level[0] = '\0';

    if (!command && !tags && !nick)
        return NULL;

    if (command && command[0])
    {
        log_level = irc_protocol_log_level_for_command (command);
        if (log_level > 0)
        {
            snprintf (str_log_level, sizeof (str_log_level),
                      ",log%d", log_level);
        }
    }

    snprintf (string, sizeof (string),
              "%s%s%s%s%s%s%s%s%s",
              (command && command[0]) ? "irc_" : "",
              (command && command[0]) ? command : "",
              (tags && tags[0]) ? "," : "",
              (tags && tags[0]) ? tags : "",
              (nick && nick[0]) ? ",nick_" : "",
              (nick && nick[0]) ? nick : "",
              (address && address[0]) ? ",host_" : "",
              (address && address[0]) ? address : "",
              str_log_level);

    return string;
}