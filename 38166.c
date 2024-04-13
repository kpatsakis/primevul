init_realm(kdc_realm_t *rdp, char *realm, char *def_mpname,
           krb5_enctype def_enctype, char *def_udp_ports, char *def_tcp_ports,
           krb5_boolean def_manual, krb5_boolean def_restrict_anon,
           char **db_args, char *no_refrls, char *host_based_srvcs)
{
    krb5_error_code     kret;
    krb5_boolean        manual;
    krb5_realm_params   *rparams;
    int                 kdb_open_flags;
    krb5_kvno       mkvno = IGNORE_VNO;

    memset(rdp, 0, sizeof(kdc_realm_t));
    if (!realm) {
        kret = EINVAL;
        goto whoops;
    }

    rdp->realm_name = strdup(realm);
    if (rdp->realm_name == NULL) {
        kret = ENOMEM;
        goto whoops;
    }
    kret = krb5int_init_context_kdc(&rdp->realm_context);
    if (kret) {
        kdc_err(NULL, kret, _("while getting context for realm %s"), realm);
        goto whoops;
    }

    kret = krb5_read_realm_params(rdp->realm_context, rdp->realm_name,
                                  &rparams);
    if (kret) {
        kdc_err(rdp->realm_context, kret, _("while reading realm parameters"));
        goto whoops;
    }

    /* Handle profile file name */
    if (rparams && rparams->realm_profile) {
        rdp->realm_profile = strdup(rparams->realm_profile);
        if (!rdp->realm_profile) {
            kret = ENOMEM;
            goto whoops;
        }
    }

    /* Handle master key name */
    if (rparams && rparams->realm_mkey_name)
        rdp->realm_mpname = strdup(rparams->realm_mkey_name);
    else
        rdp->realm_mpname = (def_mpname) ? strdup(def_mpname) :
            strdup(KRB5_KDB_M_NAME);
    if (!rdp->realm_mpname) {
        kret = ENOMEM;
        goto whoops;
    }

    /* Handle KDC ports */
    if (rparams && rparams->realm_kdc_ports)
        rdp->realm_ports = strdup(rparams->realm_kdc_ports);
    else
        rdp->realm_ports = strdup(def_udp_ports);
    if (!rdp->realm_ports) {
        kret = ENOMEM;
        goto whoops;
    }
    if (rparams && rparams->realm_kdc_tcp_ports)
        rdp->realm_tcp_ports = strdup(rparams->realm_kdc_tcp_ports);
    else
        rdp->realm_tcp_ports = strdup(def_tcp_ports);
    if (!rdp->realm_tcp_ports) {
        kret = ENOMEM;
        goto whoops;
    }
    /* Handle stash file */
    if (rparams && rparams->realm_stash_file) {
        rdp->realm_stash = strdup(rparams->realm_stash_file);
        if (!rdp->realm_stash) {
            kret = ENOMEM;
            goto whoops;
        }
        manual = FALSE;
    } else
        manual = def_manual;

    if (rparams && rparams->realm_restrict_anon_valid)
        rdp->realm_restrict_anon = rparams->realm_restrict_anon;
    else
        rdp->realm_restrict_anon = def_restrict_anon;

    /* Handle master key type */
    if (rparams && rparams->realm_enctype_valid)
        rdp->realm_mkey.enctype = (krb5_enctype) rparams->realm_enctype;
    else
        rdp->realm_mkey.enctype = manual ? def_enctype : ENCTYPE_UNKNOWN;

    /* Handle reject-bad-transit flag */
    if (rparams && rparams->realm_reject_bad_transit_valid)
        rdp->realm_reject_bad_transit = rparams->realm_reject_bad_transit;
    else
        rdp->realm_reject_bad_transit = 1;

    /* Handle ticket maximum life */
    rdp->realm_maxlife = (rparams && rparams->realm_max_life_valid) ?
        rparams->realm_max_life : KRB5_KDB_MAX_LIFE;

    /* Handle ticket renewable maximum life */
    rdp->realm_maxrlife = (rparams && rparams->realm_max_rlife_valid) ?
        rparams->realm_max_rlife : KRB5_KDB_MAX_RLIFE;

    /* Handle KDC referrals */
    kret = handle_referral_params(rparams, no_refrls, host_based_srvcs, rdp);
    if (kret == ENOMEM)
        goto whoops;

    if (rparams)
        krb5_free_realm_params(rdp->realm_context, rparams);

    /*
     * We've got our parameters, now go and setup our realm context.
     */

    /* Set the default realm of this context */
    if ((kret = krb5_set_default_realm(rdp->realm_context, realm))) {
        kdc_err(rdp->realm_context, kret,
                _("while setting default realm to %s"), realm);
        goto whoops;
    }

    /* first open the database  before doing anything */
    kdb_open_flags = KRB5_KDB_OPEN_RW | KRB5_KDB_SRV_TYPE_KDC;
    if ((kret = krb5_db_open(rdp->realm_context, db_args, kdb_open_flags))) {
        kdc_err(rdp->realm_context, kret,
                _("while initializing database for realm %s"), realm);
        goto whoops;
    }

    /* Assemble and parse the master key name */
    if ((kret = krb5_db_setup_mkey_name(rdp->realm_context, rdp->realm_mpname,
                                        rdp->realm_name, (char **) NULL,
                                        &rdp->realm_mprinc))) {
        kdc_err(rdp->realm_context, kret,
                _("while setting up master key name %s for realm %s"),
                rdp->realm_mpname, realm);
        goto whoops;
    }

    /*
     * Get the master key (note, may not be the most current mkey).
     */
    if ((kret = krb5_db_fetch_mkey(rdp->realm_context, rdp->realm_mprinc,
                                   rdp->realm_mkey.enctype, manual,
                                   FALSE, rdp->realm_stash,
                                   &mkvno, NULL, &rdp->realm_mkey))) {
        kdc_err(rdp->realm_context, kret,
                _("while fetching master key %s for realm %s"),
                rdp->realm_mpname, realm);
        goto whoops;
    }

    if ((kret = krb5_db_fetch_mkey_list(rdp->realm_context, rdp->realm_mprinc,
                                        &rdp->realm_mkey, mkvno, &rdp->mkey_list))) {
        kdc_err(rdp->realm_context, kret,
                _("while fetching master keys list for realm %s"), realm);
        goto whoops;
    }


    /* Set up the keytab */
    if ((kret = krb5_ktkdb_resolve(rdp->realm_context, NULL,
                                   &rdp->realm_keytab))) {
        kdc_err(rdp->realm_context, kret,
                _("while resolving kdb keytab for realm %s"), realm);
        goto whoops;
    }

    /* Preformat the TGS name */
    if ((kret = krb5_build_principal(rdp->realm_context, &rdp->realm_tgsprinc,
                                     strlen(realm), realm, KRB5_TGS_NAME,
                                     realm, (char *) NULL))) {
        kdc_err(rdp->realm_context, kret,
                _("while building TGS name for realm %s"), realm);
        goto whoops;
    }

    if (!rkey_init_done) {
        krb5_data seed;
        /*
         * If all that worked, then initialize the random key
         * generators.
         */

        seed.length = rdp->realm_mkey.length;
        seed.data = (char *)rdp->realm_mkey.contents;

        if ((kret = krb5_c_random_add_entropy(rdp->realm_context,
                                              KRB5_C_RANDSOURCE_TRUSTEDPARTY, &seed)))
            goto whoops;

        rkey_init_done = 1;
    }
whoops:
    /*
     * If we choked, then clean up any dirt we may have dropped on the floor.
     */
    if (kret) {

        finish_realm(rdp);
    }
    return(kret);
}
