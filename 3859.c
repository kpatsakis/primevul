IRC_PROTOCOL_CALLBACK(317)
{
    int idle_time, day, hour, min, sec;
    time_t datetime;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(6);

    idle_time = atoi (argv[4]);
    day = idle_time / (60 * 60 * 24);
    hour = (idle_time % (60 * 60 * 24)) / (60 * 60);
    min = ((idle_time % (60 * 60 * 24)) % (60 * 60)) / 60;
    sec = ((idle_time % (60 * 60 * 24)) % (60 * 60)) % 60;

    datetime = (time_t)(atol (argv[5]));

    ptr_buffer = irc_msgbuffer_get_target_buffer (server, argv[3],
                                                  command, "whois", NULL);

    if (day > 0)
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            _("%s%s[%s%s%s]%s idle: %s%d %s%s, %s%02d %s%s %s%02d %s%s %s%02d "
              "%s%s, signon at: %s%s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[3]),
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            IRC_COLOR_CHAT_CHANNEL,
            day,
            IRC_COLOR_RESET,
            NG_("day", "days", day),
            IRC_COLOR_CHAT_CHANNEL,
            hour,
            IRC_COLOR_RESET,
            NG_("hour", "hours", hour),
            IRC_COLOR_CHAT_CHANNEL,
            min,
            IRC_COLOR_RESET,
            NG_("minute", "minutes", min),
            IRC_COLOR_CHAT_CHANNEL,
            sec,
            IRC_COLOR_RESET,
            NG_("second", "seconds", sec),
            IRC_COLOR_CHAT_CHANNEL,
            weechat_util_get_time_string (&datetime));
    }
    else
    {
        weechat_printf_date_tags (
            ptr_buffer,
            date,
            irc_protocol_tags (command, "irc_numeric", NULL, NULL),
            _("%s%s[%s%s%s]%s idle: %s%02d %s%s %s%02d %s%s %s%02d %s%s, "
              "signon at: %s%s"),
            weechat_prefix ("network"),
            IRC_COLOR_CHAT_DELIMITERS,
            irc_nick_color_for_msg (server, 1, NULL, argv[3]),
            argv[3],
            IRC_COLOR_CHAT_DELIMITERS,
            IRC_COLOR_RESET,
            IRC_COLOR_CHAT_CHANNEL,
            hour,
            IRC_COLOR_RESET,
            NG_("hour", "hours", hour),
            IRC_COLOR_CHAT_CHANNEL,
            min,
            IRC_COLOR_RESET,
            NG_("minute", "minutes", min),
            IRC_COLOR_CHAT_CHANNEL,
            sec,
            IRC_COLOR_RESET,
            NG_("second", "seconds", sec),
            IRC_COLOR_CHAT_CHANNEL,
            weechat_util_get_time_string (&datetime));
    }

    return WEECHAT_RC_OK;
}