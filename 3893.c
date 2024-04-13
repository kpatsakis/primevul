irc_protocol_is_numeric_command (const char *str)
{
    while (str && str[0])
    {
        if (!isdigit ((unsigned char)str[0]))
            return 0;
        str++;
    }
    return 1;
}