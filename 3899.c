IRC_PROTOCOL_CALLBACK(authenticate)
{
    int sasl_mechanism;
    char *sasl_username, *sasl_password, *answer;
    const char *sasl_key;

    IRC_PROTOCOL_MIN_ARGS(2);

    if (irc_server_sasl_enabled (server))
    {
        sasl_mechanism = IRC_SERVER_OPTION_INTEGER(
            server, IRC_SERVER_OPTION_SASL_MECHANISM);
        sasl_username = irc_server_eval_expression (
            server,
            IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_SASL_USERNAME));
        sasl_password = irc_server_eval_expression (
            server,
            IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_SASL_PASSWORD));
        sasl_key = IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_SASL_KEY);
        answer = NULL;
        switch (sasl_mechanism)
        {
            case IRC_SASL_MECHANISM_PLAIN:
                answer = irc_sasl_mechanism_plain (sasl_username,
                                                   sasl_password);
                break;
            case IRC_SASL_MECHANISM_ECDSA_NIST256P_CHALLENGE:
                answer = irc_sasl_mechanism_ecdsa_nist256p_challenge (
                    server, argv[1], sasl_username, sasl_key);
                break;
            case IRC_SASL_MECHANISM_EXTERNAL:
                answer = strdup ("+");
                break;
            case IRC_SASL_MECHANISM_DH_BLOWFISH:
                answer = irc_sasl_mechanism_dh_blowfish (
                    argv[1], sasl_username, sasl_password);
                break;
            case IRC_SASL_MECHANISM_DH_AES:
                answer = irc_sasl_mechanism_dh_aes (
                    argv[1], sasl_username, sasl_password);
                break;
        }
        if (answer)
        {
            irc_server_sendf (server, 0, NULL, "AUTHENTICATE %s", answer);
            free (answer);
        }
        else
        {
            weechat_printf (
                server->buffer,
                _("%s%s: error building answer for SASL authentication, "
                  "using mechanism \"%s\""),
                weechat_prefix ("error"), IRC_PLUGIN_NAME,
                irc_sasl_mechanism_string[IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_SASL_MECHANISM)]);
            irc_server_sendf (server, 0, NULL, "CAP END");
        }
        if (sasl_username)
            free (sasl_username);
        if (sasl_password)
            free (sasl_password);
    }

    return WEECHAT_RC_OK;
}