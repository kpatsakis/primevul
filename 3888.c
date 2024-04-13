irc_protocol_cap_sync (struct t_irc_server *server, int sasl)
{
    char *cap_option, *cap_req, **caps_requested;
    const char *ptr_cap_option;
    int sasl_requested, sasl_to_do, sasl_fail;
    int i, length, num_caps_requested;

    sasl_requested = (sasl) ? irc_server_sasl_enabled (server) : 0;
    sasl_to_do = 0;

    ptr_cap_option = IRC_SERVER_OPTION_STRING(
        server,
        IRC_SERVER_OPTION_CAPABILITIES);
    length = ((ptr_cap_option && ptr_cap_option[0]) ?
              strlen (ptr_cap_option) : 0) + 16;
    cap_option = malloc (length);
    cap_req = malloc (length);
    if (cap_option && cap_req)
    {
        cap_option[0] = '\0';
        if (ptr_cap_option && ptr_cap_option[0])
            strcat (cap_option, ptr_cap_option);
        if (sasl && sasl_requested)
        {
            if (cap_option[0])
                strcat (cap_option, ",");
            strcat (cap_option, "sasl");
        }
        cap_req[0] = '\0';
        caps_requested = weechat_string_split (
            cap_option,
            ",",
            NULL,
            WEECHAT_STRING_SPLIT_STRIP_LEFT
            | WEECHAT_STRING_SPLIT_STRIP_RIGHT
            | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
            0,
            &num_caps_requested);
        if (caps_requested)
        {
            for (i = 0; i < num_caps_requested; i++)
            {
                if (weechat_hashtable_has_key (server->cap_ls,
                                               caps_requested[i]) &&
                    !weechat_hashtable_has_key (server->cap_list,
                                               caps_requested[i]))
                {
                    if (sasl && strcmp (caps_requested[i], "sasl") == 0)
                        sasl_to_do = 1;
                    if (cap_req[0])
                        strcat (cap_req, " ");
                    strcat (cap_req, caps_requested[i]);
                }
            }
            weechat_string_free_split (caps_requested);
        }

        if (cap_req[0])
        {
            weechat_printf (
                server->buffer,
                _("%s%s: client capability, requesting: %s"),
                weechat_prefix ("network"), IRC_PLUGIN_NAME,
                cap_req);
            irc_server_sendf (server, 0, NULL,
                              "CAP REQ :%s", cap_req);
        }

        if (sasl)
        {
            if (!sasl_to_do)
                irc_server_sendf (server, 0, NULL, "CAP END");
            if (sasl_requested && !sasl_to_do)
            {
                weechat_printf (
                    server->buffer,
                    _("%s%s: client capability: SASL not supported"),
                    weechat_prefix ("network"), IRC_PLUGIN_NAME);

                if (weechat_config_boolean (irc_config_network_sasl_fail_unavailable))
                {
                    /* same handling as for sasl_end_fail */
                    sasl_fail = IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_SASL_FAIL);
                    if ((sasl_fail == IRC_SERVER_SASL_FAIL_RECONNECT)
                        || (sasl_fail == IRC_SERVER_SASL_FAIL_DISCONNECT))
                    {
                        irc_server_disconnect (
                            server, 0,
                            (sasl_fail == IRC_SERVER_SASL_FAIL_RECONNECT) ? 1 : 0);
                    }
                }
            }
        }
    }
    if (cap_option)
        free (cap_option);
    if (cap_req)
        free (cap_req);
}