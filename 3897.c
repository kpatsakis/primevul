IRC_PROTOCOL_CALLBACK(cap)
{
    char *ptr_caps, **caps_supported, **caps_added, **caps_removed;
    char **caps_enabled, *pos_value, *str_name, **str_caps;
    char str_msg_auth[512], **str_caps_enabled, **str_caps_disabled;
    int num_caps_supported, num_caps_added, num_caps_removed;
    int num_caps_enabled, sasl_to_do, sasl_mechanism;
    int i, timeout, last_reply;

    IRC_PROTOCOL_MIN_ARGS(4);

    if (strcmp (argv[3], "LS") == 0)
    {
        if (argc > 4)
        {
            if (argc > 5 && (strcmp (argv[4], "*") == 0))
            {
                ptr_caps = argv_eol[5];
                last_reply = 0;
            }
            else
            {
                ptr_caps = argv_eol[4];
                last_reply = 1;
            }

            if (!server->checking_cap_ls)
            {
                weechat_hashtable_remove_all (server->cap_ls);
                server->checking_cap_ls = 1;
            }

            if (last_reply)
                server->checking_cap_ls = 0;

            if (ptr_caps[0] == ':')
                ptr_caps++;

            caps_supported = weechat_string_split (
                ptr_caps,
                " ",
                NULL,
                WEECHAT_STRING_SPLIT_STRIP_LEFT
                | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                0,
                &num_caps_supported);
            if (caps_supported)
            {
                for (i = 0; i < num_caps_supported; i++)
                {
                    pos_value = strstr (caps_supported[i], "=");
                    if (pos_value)
                    {
                        str_name = strndup (caps_supported[i],
                                            pos_value - caps_supported[i]);
                        if (str_name)
                        {
                            weechat_hashtable_set (server->cap_ls,
                                                   str_name, pos_value + 1);
                            free (str_name);
                        }
                    }
                    else
                    {
                        weechat_hashtable_set (server->cap_ls,
                                               caps_supported[i], NULL);
                    }
                }
            }

            if (last_reply)
            {
                str_caps = weechat_string_dyn_alloc (128);
                weechat_hashtable_map_string (server->cap_ls,
                                              irc_protocol_cap_print_cb,
                                              str_caps);
                weechat_printf_date_tags (
                    server->buffer, date, NULL,
                    _("%s%s: client capability, server supports: %s"),
                    weechat_prefix ("network"),
                    IRC_PLUGIN_NAME,
                    *str_caps);
                weechat_string_dyn_free (str_caps, 1);
            }

            /* auto-enable capabilities only when connecting to server */
            if (last_reply && !server->is_connected)
                irc_protocol_cap_sync (server, 1);

            if (caps_supported)
                weechat_string_free_split (caps_supported);
        }
    }
    else if (strcmp (argv[3], "LIST") == 0)
    {
        if (argc > 4)
        {
            if (argc > 5 && (strcmp (argv[4], "*") == 0))
            {
                ptr_caps = argv_eol[5];
                last_reply = 0;
            }
            else
            {
                ptr_caps = argv_eol[4];
                last_reply = 1;
            }

            if (!server->checking_cap_list)
            {
                weechat_hashtable_remove_all (server->cap_list);
                server->checking_cap_list = 1;
            }

            if (last_reply)
                server->checking_cap_list = 0;

            if (ptr_caps[0] == ':')
                ptr_caps++;

            caps_enabled = weechat_string_split (
                ptr_caps,
                " ",
                NULL,
                WEECHAT_STRING_SPLIT_STRIP_LEFT
                | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                0,
                &num_caps_enabled);
            if (caps_enabled)
            {
                for (i = 0; i < num_caps_enabled; i++)
                {
                    pos_value = strstr (caps_enabled[i], "=");
                    if (pos_value)
                    {
                        str_name = strndup (caps_enabled[i],
                                            pos_value - caps_enabled[i]);
                        if (str_name)
                        {
                            weechat_hashtable_set (server->cap_list,
                                                   str_name, pos_value + 1);
                            free (str_name);
                        }
                    }
                    else
                    {
                        weechat_hashtable_set (server->cap_list,
                                               caps_enabled[i], NULL);
                    }
                }
            }

            if (last_reply)
            {
                str_caps = weechat_string_dyn_alloc (128);
                weechat_hashtable_map_string (server->cap_list,
                                              irc_protocol_cap_print_cb,
                                              str_caps);
                weechat_printf_date_tags (
                    server->buffer, date, NULL,
                    _("%s%s: client capability, currently enabled: %s"),
                    weechat_prefix ("network"),
                    IRC_PLUGIN_NAME,
                    *str_caps);
                weechat_string_dyn_free (str_caps, 1);
            }

            if (caps_enabled)
                weechat_string_free_split (caps_enabled);
        }
    }
    else if (strcmp (argv[3], "ACK") == 0)
    {
        if (argc > 4)
        {
            ptr_caps = (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4];

            sasl_to_do = 0;
            str_caps_enabled = weechat_string_dyn_alloc (128);
            str_caps_disabled = weechat_string_dyn_alloc (128);

            caps_supported = weechat_string_split (
                ptr_caps,
                " ",
                NULL,
                WEECHAT_STRING_SPLIT_STRIP_LEFT
                | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                0,
                &num_caps_supported);
            if (caps_supported)
            {
                for (i = 0; i < num_caps_supported; i++)
                {
                    if (caps_supported[i][0] == '-')
                    {
                        if (*str_caps_disabled[0])
                            weechat_string_dyn_concat (str_caps_disabled, " ");
                        weechat_string_dyn_concat (str_caps_disabled,
                                                   caps_supported[i] + 1);

                        weechat_hashtable_remove (server->cap_list,
                                                  caps_supported[i] + 1);
                    }
                    else
                    {
                        if (*str_caps_enabled[0])
                            weechat_string_dyn_concat (str_caps_enabled, " ");
                        weechat_string_dyn_concat (str_caps_enabled,
                                                   caps_supported[i]);

                        weechat_hashtable_set (server->cap_list,
                                               caps_supported[i], NULL);

                        if (strcmp (caps_supported[i], "sasl") == 0)
                            sasl_to_do = 1;
                    }
                }
                weechat_string_free_split (caps_supported);
            }
            if (*str_caps_enabled[0] && *str_caps_disabled[0])
            {
                weechat_printf_date_tags (
                    server->buffer, date, NULL,
                    _("%s%s: client capability, enabled: %s, disabled: %s"),
                    weechat_prefix ("network"), IRC_PLUGIN_NAME,
                    *str_caps_enabled, *str_caps_disabled);
            }
            else if (*str_caps_enabled[0])
            {
                weechat_printf_date_tags (
                    server->buffer, date, NULL,
                    _("%s%s: client capability, enabled: %s"),
                    weechat_prefix ("network"), IRC_PLUGIN_NAME,
                    *str_caps_enabled);
            }
            else if (*str_caps_disabled[0])
            {
                weechat_printf_date_tags (
                    server->buffer, date, NULL,
                    _("%s%s: client capability, disabled: %s"),
                    weechat_prefix ("network"), IRC_PLUGIN_NAME,
                    *str_caps_disabled);
            }
            weechat_string_dyn_free (str_caps_enabled, 1);
            weechat_string_dyn_free (str_caps_disabled, 1);

            if (sasl_to_do)
            {
                sasl_mechanism = IRC_SERVER_OPTION_INTEGER(
                    server, IRC_SERVER_OPTION_SASL_MECHANISM);
                if ((sasl_mechanism >= 0)
                    && (sasl_mechanism < IRC_NUM_SASL_MECHANISMS))
                {
                    snprintf (str_msg_auth, sizeof (str_msg_auth),
                              "AUTHENTICATE %s",
                              irc_sasl_mechanism_string[sasl_mechanism]);
                    weechat_string_toupper (str_msg_auth);
                    irc_server_sendf (server, 0, NULL, str_msg_auth);
                    if (server->hook_timer_sasl)
                        weechat_unhook (server->hook_timer_sasl);
                    timeout = IRC_SERVER_OPTION_INTEGER(
                        server, IRC_SERVER_OPTION_SASL_TIMEOUT);
                    server->hook_timer_sasl = weechat_hook_timer (
                        timeout * 1000,
                        0, 1,
                        &irc_server_timer_sasl_cb,
                        server, NULL);
                }
            }
        }
    }
    else if (strcmp (argv[3], "NAK") == 0)
    {
        if (argc > 4)
        {
            ptr_caps = (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4];
            weechat_printf_date_tags (
                server->buffer, date, NULL,
                _("%s%s: client capability, refused: %s"),
                weechat_prefix ("error"), IRC_PLUGIN_NAME, ptr_caps);
            if (!server->is_connected)
                irc_server_sendf (server, 0, NULL, "CAP END");
        }
    }
    else if (strcmp (argv[3], "NEW") == 0)
    {
        if (argc > 4)
        {
            ptr_caps = (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4];
            weechat_printf_date_tags (
                server->buffer, date, NULL,
                _("%s%s: client capability, now available: %s"),
                weechat_prefix ("network"), IRC_PLUGIN_NAME, ptr_caps);
            caps_added = weechat_string_split (
                ptr_caps,
                " ",
                NULL,
                WEECHAT_STRING_SPLIT_STRIP_LEFT
                | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                0,
                &num_caps_added);
            if (caps_added)
            {
                for (i = 0; i < num_caps_added; i++)
                {
                    pos_value = strstr (caps_added[i], "=");
                    if (pos_value)
                    {
                        str_name = strndup (caps_added[i],
                                            pos_value - caps_added[i]);
                        if (str_name)
                        {
                            weechat_hashtable_set (server->cap_ls,
                                                   str_name, pos_value + 1);
                            free (str_name);
                        }
                    }
                    else
                    {
                        weechat_hashtable_set (server->cap_ls,
                                               caps_added[i], NULL);
                    }
                }
                weechat_string_free_split (caps_added);
            }

            /* TODO: SASL Reauthentication */
            irc_protocol_cap_sync (server, 0);
        }
    }
    else if (strcmp (argv[3], "DEL") == 0)
    {
        if (argc > 4)
        {
            ptr_caps = (argv_eol[4][0] == ':') ? argv_eol[4] + 1 : argv_eol[4];
            weechat_printf_date_tags (
                server->buffer, date, NULL,
                _("%s%s: client capability, removed: %s"),
                weechat_prefix ("network"), IRC_PLUGIN_NAME, ptr_caps);
            caps_removed = weechat_string_split (
                ptr_caps,
                " ",
                NULL,
                WEECHAT_STRING_SPLIT_STRIP_LEFT
                | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                0,
                &num_caps_removed);
            if (caps_removed)
            {
                for (i = 0; i < num_caps_removed; i++)
                {
                    weechat_hashtable_remove (server->cap_ls, caps_removed[i]);
                    weechat_hashtable_remove (server->cap_list, caps_removed[i]);
                }
                weechat_string_free_split (caps_removed);
            }
        }
    }

    return WEECHAT_RC_OK;
}