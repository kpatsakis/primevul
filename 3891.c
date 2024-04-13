IRC_PROTOCOL_CALLBACK(729)
{
    char *pos_args;
    struct t_irc_channel *ptr_channel;
    struct t_gui_buffer *ptr_buffer;
    struct t_irc_modelist *ptr_modelist;

    IRC_PROTOCOL_MIN_ARGS(5);

    pos_args = (argc > 5) ?
        ((argv_eol[5][0] == ':') ? argv_eol[5] + 1 : argv_eol[5]) : NULL;

    ptr_channel = irc_channel_search (server, argv[3]);
    ptr_buffer = (ptr_channel && ptr_channel->nicks) ?
        ptr_channel->buffer : server->buffer;
    ptr_modelist = irc_modelist_search (ptr_channel, argv[4][0]);
    if (ptr_modelist)
    {
        if (ptr_modelist->state != IRC_MODELIST_STATE_RECEIVING)
        {
            /*
             * remove all items if no quiet was received before
             * the end of quiet list
             */
            irc_modelist_item_free_all (ptr_modelist);
        }
        ptr_modelist->state = IRC_MODELIST_STATE_RECEIVED;
    }
    weechat_printf_date_tags (
        irc_msgbuffer_get_target_buffer (
            server, NULL, command, "quietlist", ptr_buffer),
        date,
        irc_protocol_tags (command, "irc_numeric", NULL, NULL),
        "%s%s[%s%s%s]%s%s%s",
        weechat_prefix ("network"),
        IRC_COLOR_CHAT_DELIMITERS,
        IRC_COLOR_CHAT_CHANNEL,
        argv[3],
        IRC_COLOR_CHAT_DELIMITERS,
        IRC_COLOR_RESET,
        (pos_args) ? " " : "",
        (pos_args) ? pos_args : "");

    return WEECHAT_RC_OK;
}