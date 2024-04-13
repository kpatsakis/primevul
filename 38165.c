handle_referral_params(krb5_realm_params *rparams,
                       char *no_refrls, char *host_based_srvcs,
                       kdc_realm_t *rdp )
{
    krb5_error_code retval = 0;
    if (no_refrls && krb5_match_config_pattern(no_refrls, KRB5_CONF_ASTERISK) == TRUE) {
        rdp->realm_no_host_referral = strdup(KRB5_CONF_ASTERISK);
        if (!rdp->realm_no_host_referral)
            retval = ENOMEM;
    } else {
        if (rparams && rparams->realm_no_host_referral) {
            if (krb5_match_config_pattern(rparams->realm_no_host_referral,
                                          KRB5_CONF_ASTERISK) == TRUE) {
                rdp->realm_no_host_referral = strdup(KRB5_CONF_ASTERISK);
                if (!rdp->realm_no_host_referral)
                    retval = ENOMEM;
            } else if (no_refrls) {
                if (asprintf(&(rdp->realm_no_host_referral),
                             "%s%s%s%s%s", " ", no_refrls," ",
                             rparams->realm_no_host_referral, " ") < 0)
                    retval = ENOMEM;
            } else if (asprintf(&(rdp->realm_no_host_referral),"%s%s%s", " ",
                                rparams->realm_no_host_referral, " ") < 0)
                retval = ENOMEM;
        } else if( no_refrls != NULL) {
            if ( asprintf(&(rdp->realm_no_host_referral),
                          "%s%s%s", " ", no_refrls, " ") < 0)
                retval = ENOMEM;
        } else
            rdp->realm_no_host_referral = NULL;
    }

    if (rdp->realm_no_host_referral &&
        krb5_match_config_pattern(rdp->realm_no_host_referral,
                                  KRB5_CONF_ASTERISK) == TRUE) {
        rdp->realm_host_based_services = NULL;
        return 0;
    }

    if (host_based_srvcs &&
        (krb5_match_config_pattern(host_based_srvcs, KRB5_CONF_ASTERISK) == TRUE)) {
        rdp->realm_host_based_services = strdup(KRB5_CONF_ASTERISK);
        if (!rdp->realm_host_based_services)
            retval = ENOMEM;
    } else {
        if (rparams && rparams->realm_host_based_services) {
            if (krb5_match_config_pattern(rparams->realm_host_based_services,
                                          KRB5_CONF_ASTERISK) == TRUE) {
                rdp->realm_host_based_services = strdup(KRB5_CONF_ASTERISK);
                if (!rdp->realm_host_based_services)
                    retval = ENOMEM;
            } else if (host_based_srvcs) {
                if (asprintf(&(rdp->realm_host_based_services), "%s%s%s%s%s",
                             " ", host_based_srvcs," ",
                             rparams->realm_host_based_services, " ") < 0)
                    retval = ENOMEM;
            } else if (asprintf(&(rdp->realm_host_based_services),"%s%s%s", " ",
                                rparams->realm_host_based_services, " ") < 0)
                retval = ENOMEM;
        } else if (host_based_srvcs) {
            if (asprintf(&(rdp->realm_host_based_services),"%s%s%s", " ",
                         host_based_srvcs, " ") < 0)
                retval = ENOMEM;
        } else
            rdp->realm_host_based_services = NULL;
    }

    return retval;
}
