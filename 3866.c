IRC_PROTOCOL_CALLBACK(331)
{
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;

    IRC_PROTOCOL_MIN_ARGS(4);

    ptr_channel = irc_channel_search (server, argv[3]);
    ptr_buffer = (ptr_channel) ? ptr_channel->buffer : server->buffer;
    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, argv[3], command, NULL, ptr_buffer),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        _("%sNo topic set for channel %s%s"),
        weechat_prefix ("network"),
        IRC_COLOR_CHAT_CHANNEL,
        argv[3]);

    return WEECHAT_RC_OK;
}