irc_protocol_recv_command (struct t_irc_server *server,
                           const char *irc_message,
                           const char *msg_command,
                           const char *msg_channel)
{
    int i, cmd_found, return_code, argc, decode_color, keep_trailing_spaces;
    int message_ignored, flags;
    char *message_colors_decoded, *pos_space, *tags;
    struct t_irc_channel *ptr_channel;
    t_irc_recv_func *cmd_recv_func;
    const char *cmd_name, *ptr_msg_after_tags;
    time_t date;
    const char *nick1, *address1, *host1;
    char *nick, *address, *address_color, *host, *host_no_color, *host_color;
    char **argv, **argv_eol;
    struct t_hashtable *hash_tags;
    struct t_irc_protocol_msg irc_protocol_messages[] =
        { { "account", /* account (cap account-notify) */ 1, 0, &irc_protocol_cb_account },
          { "authenticate", /* authenticate */ 1, 0, &irc_protocol_cb_authenticate },
          { "away", /* away (cap away-notify) */ 1, 0, &irc_protocol_cb_away },
          { "cap", /* client capability */ 1, 0, &irc_protocol_cb_cap },
          { "chghost", /* user/host change (cap chghost) */ 1, 0, &irc_protocol_cb_chghost },
          { "error", /* error received from IRC server */ 1, 0, &irc_protocol_cb_error },
          { "invite", /* invite a nick on a channel */ 1, 0, &irc_protocol_cb_invite },
          { "join", /* join a channel */ 1, 0, &irc_protocol_cb_join },
          { "kick", /* forcibly remove a user from a channel */ 1, 1, &irc_protocol_cb_kick },
          { "kill", /* close client-server connection */ 1, 1, &irc_protocol_cb_kill },
          { "mode", /* change channel or user mode */ 1, 0, &irc_protocol_cb_mode },
          { "nick", /* change current nickname */ 1, 0, &irc_protocol_cb_nick },
          { "notice", /* send notice message to user */ 1, 1, &irc_protocol_cb_notice },
          { "part", /* leave a channel */ 1, 1, &irc_protocol_cb_part },
          { "ping", /* ping server */ 1, 0, &irc_protocol_cb_ping },
          { "pong", /* answer to a ping message */ 1, 0, &irc_protocol_cb_pong },
          { "privmsg", /* message received */ 1, 1, &irc_protocol_cb_privmsg },
          { "quit", /* close all connections and quit */ 1, 1, &irc_protocol_cb_quit },
          { "topic", /* get/set channel topic */ 0, 1, &irc_protocol_cb_topic },
          { "wallops", /* send a message to all currently connected users who have "
                          "set the 'w' user mode "
                          "for themselves */ 1, 1, &irc_protocol_cb_wallops },
          { "001", /* a server message */ 1, 0, &irc_protocol_cb_001 },
          { "005", /* a server message */ 1, 0, &irc_protocol_cb_005 },
          { "008", /* server notice mask */ 1, 0, &irc_protocol_cb_008 },
          { "221", /* user mode string */ 1, 0, &irc_protocol_cb_221 },
          { "223", /* whois (charset is) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "264", /* whois (is using encrypted connection) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "275", /* whois (secure connection) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "276", /* whois (has client certificate fingerprint) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "301", /* away message */ 1, 1, &irc_protocol_cb_301 },
          { "303", /* ison */ 1, 0, &irc_protocol_cb_303 },
          { "305", /* unaway */ 1, 0, &irc_protocol_cb_305 },
          { "306", /* now away */ 1, 0, &irc_protocol_cb_306 },
          { "307", /* whois (registered nick) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "310", /* whois (help mode) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "311", /* whois (user) */ 1, 0, &irc_protocol_cb_311 },
          { "312", /* whois (server) */ 1, 0, &irc_protocol_cb_312 },
          { "313", /* whois (operator) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "314", /* whowas */ 1, 0, &irc_protocol_cb_314 },
          { "315", /* end of /who list */ 1, 0, &irc_protocol_cb_315 },
          { "317", /* whois (idle) */ 1, 0, &irc_protocol_cb_317 },
          { "318", /* whois (end) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "319", /* whois (channels) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "320", /* whois (identified user) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "321", /* /list start */ 1, 0, &irc_protocol_cb_321 },
          { "322", /* channel (for /list) */ 1, 0, &irc_protocol_cb_322 },
          { "323", /* end of /list */ 1, 0, &irc_protocol_cb_323 },
          { "324", /* channel mode */ 1, 0, &irc_protocol_cb_324 },
          { "326", /* whois (has oper privs) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "327", /* whois (host) */ 1, 0, &irc_protocol_cb_327 },
          { "328", /* channel url */ 1, 0, &irc_protocol_cb_328 },
          { "329", /* channel creation date */ 1, 0, &irc_protocol_cb_329 },
          { "330", /* is logged in as */ 1, 0, &irc_protocol_cb_330_343 },
          { "331", /* no topic for channel */ 1, 0, &irc_protocol_cb_331 },
          { "332", /* topic of channel */ 0, 1, &irc_protocol_cb_332 },
          { "333", /* infos about topic (nick and date changed) */ 1, 0, &irc_protocol_cb_333 },
          { "335", /* is a bot on */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "338", /* whois (host) */ 1, 0, &irc_protocol_cb_338 },
          { "341", /* inviting */ 1, 0, &irc_protocol_cb_341 },
          { "343", /* is opered as */ 1, 0, &irc_protocol_cb_330_343 },
          { "344", /* channel reop */ 1, 0, &irc_protocol_cb_344 },
          { "345", /* end of channel reop list */ 1, 0, &irc_protocol_cb_345 },
          { "346", /* invite list */ 1, 0, &irc_protocol_cb_346 },
          { "347", /* end of invite list */ 1, 0, &irc_protocol_cb_347 },
          { "348", /* channel exception list */ 1, 0, &irc_protocol_cb_348 },
          { "349", /* end of channel exception list */ 1, 0, &irc_protocol_cb_349 },
          { "351", /* server version */ 1, 0, &irc_protocol_cb_351 },
          { "352", /* who */ 1, 0, &irc_protocol_cb_352 },
          { "353", /* list of nicks on channel */ 1, 0, &irc_protocol_cb_353 },
          { "354", /* whox */ 1, 0, &irc_protocol_cb_354 },
          { "366", /* end of /names list */ 1, 0, &irc_protocol_cb_366 },
          { "367", /* banlist */ 1, 0, &irc_protocol_cb_367 },
          { "368", /* end of banlist */ 1, 0, &irc_protocol_cb_368 },
          { "369", /* whowas (end) */ 1, 0, &irc_protocol_cb_whowas_nick_msg },
          { "378", /* whois (connecting from) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "379", /* whois (using modes) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "401", /* no such nick/channel */ 1, 0, &irc_protocol_cb_generic_error },
          { "402", /* no such server */ 1, 0, &irc_protocol_cb_generic_error },
          { "403", /* no such channel */ 1, 0, &irc_protocol_cb_generic_error },
          { "404", /* cannot send to channel */ 1, 0, &irc_protocol_cb_generic_error },
          { "405", /* too many channels */ 1, 0, &irc_protocol_cb_generic_error },
          { "406", /* was no such nick */ 1, 0, &irc_protocol_cb_generic_error },
          { "407", /* was no such nick */ 1, 0, &irc_protocol_cb_generic_error },
          { "409", /* no origin */ 1, 0, &irc_protocol_cb_generic_error },
          { "410", /* no services */ 1, 0, &irc_protocol_cb_generic_error },
          { "411", /* no recipient */ 1, 0, &irc_protocol_cb_generic_error },
          { "412", /* no text to send */ 1, 0, &irc_protocol_cb_generic_error },
          { "413", /* no toplevel */ 1, 0, &irc_protocol_cb_generic_error },
          { "414", /* wilcard in toplevel domain */ 1, 0, &irc_protocol_cb_generic_error },
          { "421", /* unknown command */ 1, 0, &irc_protocol_cb_generic_error },
          { "422", /* MOTD is missing */ 1, 0, &irc_protocol_cb_generic_error },
          { "423", /* no administrative info */ 1, 0, &irc_protocol_cb_generic_error },
          { "424", /* file error */ 1, 0, &irc_protocol_cb_generic_error },
          { "431", /* no nickname given */ 1, 0, &irc_protocol_cb_generic_error },
          { "432", /* erroneous nickname */ 1, 0, &irc_protocol_cb_432 },
          { "433", /* nickname already in use */ 1, 0, &irc_protocol_cb_433 },
          { "436", /* nickname collision */ 1, 0, &irc_protocol_cb_generic_error },
          { "437", /* nick/channel unavailable */ 1, 0, &irc_protocol_cb_437 },
          { "438", /* not authorized to change nickname */ 1, 0, &irc_protocol_cb_438 },
          { "441", /* user not in channel */ 1, 0, &irc_protocol_cb_generic_error },
          { "442", /* not on channel */ 1, 0, &irc_protocol_cb_generic_error },
          { "443", /* user already on channel */ 1, 0, &irc_protocol_cb_generic_error },
          { "444", /* user not logged in */ 1, 0, &irc_protocol_cb_generic_error },
          { "445", /* summon has been disabled */ 1, 0, &irc_protocol_cb_generic_error },
          { "446", /* users has been disabled */ 1, 0, &irc_protocol_cb_generic_error },
          { "451", /* you are not registered */ 1, 0, &irc_protocol_cb_generic_error },
          { "461", /* not enough parameters */ 1, 0, &irc_protocol_cb_generic_error },
          { "462", /* you may not register */ 1, 0, &irc_protocol_cb_generic_error },
          { "463", /* your host isn't among the privileged */ 1, 0, &irc_protocol_cb_generic_error },
          { "464", /* password incorrect */ 1, 0, &irc_protocol_cb_generic_error },
          { "465", /* you are banned from this server */ 1, 0, &irc_protocol_cb_generic_error },
          { "467", /* channel key already set */ 1, 0, &irc_protocol_cb_generic_error },
          { "470", /* forwarding to another channel */ 1, 0, &irc_protocol_cb_470 },
          { "471", /* channel is already full */ 1, 0, &irc_protocol_cb_generic_error },
          { "472", /* unknown mode char to me */ 1, 0, &irc_protocol_cb_generic_error },
          { "473", /* cannot join channel (invite only) */ 1, 0, &irc_protocol_cb_generic_error },
          { "474", /* cannot join channel (banned from channel) */ 1, 0, &irc_protocol_cb_generic_error },
          { "475", /* cannot join channel (bad channel key) */ 1, 0, &irc_protocol_cb_generic_error },
          { "476", /* bad channel mask */ 1, 0, &irc_protocol_cb_generic_error },
          { "477", /* channel doesn't support modes */ 1, 0, &irc_protocol_cb_generic_error },
          { "481", /* you're not an IRC operator */ 1, 0, &irc_protocol_cb_generic_error },
          { "482", /* you're not channel operator */ 1, 0, &irc_protocol_cb_generic_error },
          { "483", /* you can't kill a server! */ 1, 0, &irc_protocol_cb_generic_error },
          { "484", /* your connection is restricted! */ 1, 0, &irc_protocol_cb_generic_error },
          { "485", /* user is immune from kick/deop */ 1, 0, &irc_protocol_cb_generic_error },
          { "487", /* network split */ 1, 0, &irc_protocol_cb_generic_error },
          { "491", /* no O-lines for your host */ 1, 0, &irc_protocol_cb_generic_error },
          { "501", /* unknown mode flag */ 1, 0, &irc_protocol_cb_generic_error },
          { "502", /* can't change mode for other users */ 1, 0, &irc_protocol_cb_generic_error },
          { "671", /* whois (secure connection) */ 1, 0, &irc_protocol_cb_whois_nick_msg },
          { "728", /* quietlist */ 1, 0, &irc_protocol_cb_728 },
          { "729", /* end of quietlist */ 1, 0, &irc_protocol_cb_729 },
          { "730", /* monitored nicks online */ 1, 0, &irc_protocol_cb_730 },
          { "731", /* monitored nicks offline */ 1, 0, &irc_protocol_cb_731 },
          { "732", /* list of monitored nicks */ 1, 0, &irc_protocol_cb_732 },
          { "733", /* end of monitor list */ 1, 0, &irc_protocol_cb_733 },
          { "734", /* monitor list is full */ 1, 0, &irc_protocol_cb_734 },
          { "900", /* logged in as (SASL) */ 1, 0, &irc_protocol_cb_900 },
          { "901", /* you are now logged in */ 1, 0, &irc_protocol_cb_901 },
          { "902", /* SASL authentication failed (account locked/held) */ 1, 0, &irc_protocol_cb_sasl_end_fail },
          { "903", /* SASL authentication successful */ 1, 0, &irc_protocol_cb_sasl_end_ok },
          { "904", /* SASL authentication failed */ 1, 0, &irc_protocol_cb_sasl_end_fail },
          { "905", /* SASL message too long */ 1, 0, &irc_protocol_cb_sasl_end_fail },
          { "906", /* SASL authentication aborted */ 1, 0, &irc_protocol_cb_sasl_end_fail },
          { "907", /* You have already completed SASL authentication */ 1, 0, &irc_protocol_cb_sasl_end_ok },
          { "936", /* censored word */ 1, 0, &irc_protocol_cb_generic_error },
          { "973", /* whois (secure connection) */ 1, 0, &irc_protocol_cb_server_mode_reason },
          { "974", /* whois (secure connection) */ 1, 0, &irc_protocol_cb_server_mode_reason },
          { "975", /* whois (secure connection) */ 1, 0, &irc_protocol_cb_server_mode_reason },
          { NULL, 0, 0, NULL }
        };

    if (!msg_command)
        return;

    message_colors_decoded = NULL;
    argv = NULL;
    argv_eol = NULL;
    hash_tags = NULL;
    date = 0;

    ptr_msg_after_tags = irc_message;

    /* get tags as hashtable */
    if (irc_message && (irc_message[0] == '@'))
    {
        pos_space = strchr (irc_message, ' ');
        if (pos_space)
        {
            tags = weechat_strndup (irc_message + 1,
                                    pos_space - (irc_message + 1));
            if (tags)
            {
                hash_tags = irc_protocol_get_message_tags (tags);
                if (hash_tags)
                {
                    date = irc_protocol_parse_time (
                        weechat_hashtable_get (hash_tags, "time"));
                }
                free (tags);
            }
            ptr_msg_after_tags = pos_space;
            while (ptr_msg_after_tags[0] == ' ')
            {
                ptr_msg_after_tags++;
            }
        }
        else
            ptr_msg_after_tags = NULL;
    }

    /* get nick/host/address from IRC message */
    nick1 = NULL;
    address1 = NULL;
    host1 = NULL;
    if (ptr_msg_after_tags && (ptr_msg_after_tags[0] == ':'))
    {
        nick1 = irc_message_get_nick_from_host (ptr_msg_after_tags);
        address1 = irc_message_get_address_from_host (ptr_msg_after_tags);
        host1 = ptr_msg_after_tags + 1;
    }
    nick = (nick1) ? strdup (nick1) : NULL;
    address = (address1) ? strdup (address1) : NULL;
    address_color = (address) ?
        irc_color_decode (
            address,
            weechat_config_boolean (irc_config_network_colors_receive)) :
        NULL;
    host = (host1) ? strdup (host1) : NULL;
    if (host)
    {
        pos_space = strchr (host, ' ');
        if (pos_space)
            pos_space[0] = '\0';
    }
    host_no_color = (host) ? irc_color_decode (host, 0) : NULL;
    host_color = (host) ?
        irc_color_decode (
            host,
            weechat_config_boolean (irc_config_network_colors_receive)) :
        NULL;

    /* check if message is ignored or not */
    ptr_channel = NULL;
    if (msg_channel)
        ptr_channel = irc_channel_search (server, msg_channel);
    message_ignored = irc_ignore_check (
        server,
        (ptr_channel) ? ptr_channel->name : msg_channel,
        nick, host_no_color);

    /* send signal with received command, even if command is ignored */
    irc_server_send_signal (server, "irc_raw_in", msg_command,
                            irc_message, NULL);

    /* send signal with received command, only if message is not ignored */
    if (!message_ignored)
    {
        irc_server_send_signal (server, "irc_in", msg_command,
                                irc_message, NULL);
    }

    /* look for IRC command */
    cmd_found = -1;
    for (i = 0; irc_protocol_messages[i].name; i++)
    {
        if (weechat_strcasecmp (irc_protocol_messages[i].name,
                                msg_command) == 0)
        {
            cmd_found = i;
            break;
        }
    }

    /* command not found */
    if (cmd_found < 0)
    {
        /* for numeric commands, we use default recv function */
        if (irc_protocol_is_numeric_command (msg_command))
        {
            cmd_name = msg_command;
            decode_color = 1;
            keep_trailing_spaces = 0;
            cmd_recv_func = irc_protocol_cb_numeric;
        }
        else
        {
            weechat_printf (server->buffer,
                            _("%s%s: command \"%s\" not found:"),
                            weechat_prefix ("error"), IRC_PLUGIN_NAME,
                            msg_command);
            weechat_printf (server->buffer,
                            "%s%s",
                            weechat_prefix ("error"), irc_message);
            goto end;
        }
    }
    else
    {
        cmd_name = irc_protocol_messages[cmd_found].name;
        decode_color = irc_protocol_messages[cmd_found].decode_color;
        keep_trailing_spaces = irc_protocol_messages[cmd_found].keep_trailing_spaces;
        cmd_recv_func = irc_protocol_messages[cmd_found].recv_function;
    }

    if (cmd_recv_func != NULL)
    {
        if (ptr_msg_after_tags)
        {
            if (decode_color)
            {
                message_colors_decoded = irc_color_decode (
                    ptr_msg_after_tags,
                    weechat_config_boolean (irc_config_network_colors_receive));
            }
            else
            {
                message_colors_decoded = strdup (ptr_msg_after_tags);
            }
        }
        else
            message_colors_decoded = NULL;
        argv = weechat_string_split (message_colors_decoded, " ", NULL,
                                     WEECHAT_STRING_SPLIT_STRIP_LEFT
                                     | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                                     | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                                     0, &argc);
        flags = WEECHAT_STRING_SPLIT_STRIP_LEFT
            | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS
            | WEECHAT_STRING_SPLIT_KEEP_EOL;
        if (keep_trailing_spaces)
            flags |= WEECHAT_STRING_SPLIT_STRIP_RIGHT;
        argv_eol = weechat_string_split (message_colors_decoded, " ", NULL,
                                         flags, 0, NULL);

        return_code = (int) (cmd_recv_func) (server,
                                             date, nick, address_color,
                                             host_color, cmd_name,
                                             message_ignored, argc, argv,
                                             argv_eol);

        if (return_code == WEECHAT_RC_ERROR)
        {
            weechat_printf (server->buffer,
                            _("%s%s: failed to parse command \"%s\" (please "
                              "report to developers):"),
                            weechat_prefix ("error"), IRC_PLUGIN_NAME,
                            msg_command);
            weechat_printf (server->buffer,
                            "%s%s",
                            weechat_prefix ("error"), irc_message);
        }

        /* send signal with received command (if message is not ignored) */
        if (!message_ignored)
        {
            irc_server_send_signal (server, "irc_in2", msg_command,
                                    irc_message, NULL);
        }
    }

    /* send signal with received command, even if command is ignored */
    irc_server_send_signal (server, "irc_raw_in2", msg_command,
                            irc_message, NULL);

end:
    if (nick)
        free (nick);
    if (address)
        free (address);
    if (address_color)
        free (address_color);
    if (host)
        free (host);
    if (host_no_color)
        free (host_no_color);
    if (host_color)
        free (host_color);
    if (message_colors_decoded)
        free (message_colors_decoded);
    if (argv)
        weechat_string_free_split (argv);
    if (argv_eol)
        weechat_string_free_split (argv_eol);
    if (hash_tags)
        weechat_hashtable_free (hash_tags);
}