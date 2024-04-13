int MAIN(int argc, char *argv[])
{
    X509_VERIFY_PARAM *vpm = NULL;
    int badarg = 0;
    short port = PORT;
    char *CApath = NULL, *CAfile = NULL;
    char *chCApath = NULL, *chCAfile = NULL;
    char *vfyCApath = NULL, *vfyCAfile = NULL;
    unsigned char *context = NULL;
    char *dhfile = NULL;
    int badop = 0;
    int ret = 1;
    int build_chain = 0;
    int no_tmp_rsa = 0, no_dhe = 0, no_ecdhe = 0, nocert = 0;
    int state = 0;
    const SSL_METHOD *meth = NULL;
    int socket_type = SOCK_STREAM;
    ENGINE *e = NULL;
    char *inrand = NULL;
    int s_cert_format = FORMAT_PEM, s_key_format = FORMAT_PEM;
    char *passarg = NULL, *pass = NULL;
    char *dpassarg = NULL, *dpass = NULL;
    int s_dcert_format = FORMAT_PEM, s_dkey_format = FORMAT_PEM;
    X509 *s_cert = NULL, *s_dcert = NULL;
    STACK_OF(X509) *s_chain = NULL, *s_dchain = NULL;
    EVP_PKEY *s_key = NULL, *s_dkey = NULL;
    int no_cache = 0, ext_cache = 0;
    int rev = 0, naccept = -1;
#ifndef OPENSSL_NO_TLSEXT
    EVP_PKEY *s_key2 = NULL;
    X509 *s_cert2 = NULL;
    tlsextctx tlsextcbp = { NULL, NULL, SSL_TLSEXT_ERR_ALERT_WARNING };
# ifndef OPENSSL_NO_NEXTPROTONEG
    const char *next_proto_neg_in = NULL;
    tlsextnextprotoctx next_proto = { NULL, 0 };
# endif
    const char *alpn_in = NULL;
    tlsextalpnctx alpn_ctx = { NULL, 0 };
#endif
#ifndef OPENSSL_NO_PSK
    /* by default do not send a PSK identity hint */
    static char *psk_identity_hint = NULL;
#endif
#ifndef OPENSSL_NO_SRP
    char *srpuserseed = NULL;
    char *srp_verifier_file = NULL;
#endif
    SSL_EXCERT *exc = NULL;
    SSL_CONF_CTX *cctx = NULL;
    STACK_OF(OPENSSL_STRING) *ssl_args = NULL;

    char *crl_file = NULL;
    int crl_format = FORMAT_PEM;
    int crl_download = 0;
    STACK_OF(X509_CRL) *crls = NULL;

    meth = SSLv23_server_method();

    local_argc = argc;
    local_argv = argv;

    apps_startup();
#ifdef MONOLITH
    s_server_init();
#endif

    if (bio_err == NULL)
        bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);

    if (!load_config(bio_err, NULL))
        goto end;

    cctx = SSL_CONF_CTX_new();
    if (!cctx)
        goto end;
    SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_SERVER);
    SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_CMDLINE);

    verify_depth = 0;
#ifdef FIONBIO
    s_nbio = 0;
#endif
    s_nbio_test = 0;

    argc--;
    argv++;

    while (argc >= 1) {
        if ((strcmp(*argv, "-port") == 0) || (strcmp(*argv, "-accept") == 0)) {
            if (--argc < 1)
                goto bad;
            if (!extract_port(*(++argv), &port))
                goto bad;
        } else if (strcmp(*argv, "-naccept") == 0) {
            if (--argc < 1)
                goto bad;
            naccept = atol(*(++argv));
            if (naccept <= 0) {
                BIO_printf(bio_err, "bad accept value %s\n", *argv);
                goto bad;
            }
        } else if (strcmp(*argv, "-verify") == 0) {
            s_server_verify = SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE;
            if (--argc < 1)
                goto bad;
            verify_depth = atoi(*(++argv));
            if (!s_quiet)
                BIO_printf(bio_err, "verify depth is %d\n", verify_depth);
        } else if (strcmp(*argv, "-Verify") == 0) {
            s_server_verify =
                SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT |
                SSL_VERIFY_CLIENT_ONCE;
            if (--argc < 1)
                goto bad;
            verify_depth = atoi(*(++argv));
            if (!s_quiet)
                BIO_printf(bio_err,
                           "verify depth is %d, must return a certificate\n",
                           verify_depth);
        } else if (strcmp(*argv, "-context") == 0) {
            if (--argc < 1)
                goto bad;
            context = (unsigned char *)*(++argv);
        } else if (strcmp(*argv, "-cert") == 0) {
            if (--argc < 1)
                goto bad;
            s_cert_file = *(++argv);
        } else if (strcmp(*argv, "-CRL") == 0) {
            if (--argc < 1)
                goto bad;
            crl_file = *(++argv);
        } else if (strcmp(*argv, "-crl_download") == 0)
            crl_download = 1;
#ifndef OPENSSL_NO_TLSEXT
        else if (strcmp(*argv, "-serverinfo") == 0) {
            if (--argc < 1)
                goto bad;
            s_serverinfo_file = *(++argv);
        }
#endif
        else if (strcmp(*argv, "-certform") == 0) {
            if (--argc < 1)
                goto bad;
            s_cert_format = str2fmt(*(++argv));
        } else if (strcmp(*argv, "-key") == 0) {
            if (--argc < 1)
                goto bad;
            s_key_file = *(++argv);
        } else if (strcmp(*argv, "-keyform") == 0) {
            if (--argc < 1)
                goto bad;
            s_key_format = str2fmt(*(++argv));
        } else if (strcmp(*argv, "-pass") == 0) {
            if (--argc < 1)
                goto bad;
            passarg = *(++argv);
        } else if (strcmp(*argv, "-cert_chain") == 0) {
            if (--argc < 1)
                goto bad;
            s_chain_file = *(++argv);
        } else if (strcmp(*argv, "-dhparam") == 0) {
            if (--argc < 1)
                goto bad;
            dhfile = *(++argv);
        } else if (strcmp(*argv, "-dcertform") == 0) {
            if (--argc < 1)
                goto bad;
            s_dcert_format = str2fmt(*(++argv));
        } else if (strcmp(*argv, "-dcert") == 0) {
            if (--argc < 1)
                goto bad;
            s_dcert_file = *(++argv);
        } else if (strcmp(*argv, "-dkeyform") == 0) {
            if (--argc < 1)
                goto bad;
            s_dkey_format = str2fmt(*(++argv));
        } else if (strcmp(*argv, "-dpass") == 0) {
            if (--argc < 1)
                goto bad;
            dpassarg = *(++argv);
        } else if (strcmp(*argv, "-dkey") == 0) {
            if (--argc < 1)
                goto bad;
            s_dkey_file = *(++argv);
        } else if (strcmp(*argv, "-dcert_chain") == 0) {
            if (--argc < 1)
                goto bad;
            s_dchain_file = *(++argv);
        } else if (strcmp(*argv, "-nocert") == 0) {
            nocert = 1;
        } else if (strcmp(*argv, "-CApath") == 0) {
            if (--argc < 1)
                goto bad;
            CApath = *(++argv);
        } else if (strcmp(*argv, "-chainCApath") == 0) {
            if (--argc < 1)
                goto bad;
            chCApath = *(++argv);
        } else if (strcmp(*argv, "-verifyCApath") == 0) {
            if (--argc < 1)
                goto bad;
            vfyCApath = *(++argv);
        } else if (strcmp(*argv, "-no_cache") == 0)
            no_cache = 1;
        else if (strcmp(*argv, "-ext_cache") == 0)
            ext_cache = 1;
        else if (strcmp(*argv, "-CRLform") == 0) {
            if (--argc < 1)
                goto bad;
            crl_format = str2fmt(*(++argv));
        } else if (args_verify(&argv, &argc, &badarg, bio_err, &vpm)) {
            if (badarg)
                goto bad;
            continue;
        } else if (args_excert(&argv, &argc, &badarg, bio_err, &exc)) {
            if (badarg)
                goto bad;
            continue;
        } else if (args_ssl(&argv, &argc, cctx, &badarg, bio_err, &ssl_args)) {
            if (badarg)
                goto bad;
            continue;
        } else if (strcmp(*argv, "-verify_return_error") == 0)
            verify_return_error = 1;
        else if (strcmp(*argv, "-verify_quiet") == 0)
            verify_quiet = 1;
        else if (strcmp(*argv, "-build_chain") == 0)
            build_chain = 1;
        else if (strcmp(*argv, "-CAfile") == 0) {
            if (--argc < 1)
                goto bad;
            CAfile = *(++argv);
        } else if (strcmp(*argv, "-chainCAfile") == 0) {
            if (--argc < 1)
                goto bad;
            chCAfile = *(++argv);
        } else if (strcmp(*argv, "-verifyCAfile") == 0) {
            if (--argc < 1)
                goto bad;
            vfyCAfile = *(++argv);
        }
#ifdef FIONBIO
        else if (strcmp(*argv, "-nbio") == 0) {
            s_nbio = 1;
        }
#endif
        else if (strcmp(*argv, "-nbio_test") == 0) {
#ifdef FIONBIO
            s_nbio = 1;
#endif
            s_nbio_test = 1;
        } else if (strcmp(*argv, "-ign_eof") == 0)
            s_ign_eof = 1;
        else if (strcmp(*argv, "-no_ign_eof") == 0)
            s_ign_eof = 0;
        else if (strcmp(*argv, "-debug") == 0) {
            s_debug = 1;
        }
#ifndef OPENSSL_NO_TLSEXT
        else if (strcmp(*argv, "-tlsextdebug") == 0)
            s_tlsextdebug = 1;
        else if (strcmp(*argv, "-status") == 0)
            s_tlsextstatus = 1;
        else if (strcmp(*argv, "-status_verbose") == 0) {
            s_tlsextstatus = 1;
            tlscstatp.verbose = 1;
        } else if (!strcmp(*argv, "-status_timeout")) {
            s_tlsextstatus = 1;
            if (--argc < 1)
                goto bad;
            tlscstatp.timeout = atoi(*(++argv));
        } else if (!strcmp(*argv, "-status_url")) {
            s_tlsextstatus = 1;
            if (--argc < 1)
                goto bad;
            if (!OCSP_parse_url(*(++argv),
                                &tlscstatp.host,
                                &tlscstatp.port,
                                &tlscstatp.path, &tlscstatp.use_ssl)) {
                BIO_printf(bio_err, "Error parsing URL\n");
                goto bad;
            }
        }
#endif
        else if (strcmp(*argv, "-msg") == 0) {
            s_msg = 1;
        } else if (strcmp(*argv, "-msgfile") == 0) {
            if (--argc < 1)
                goto bad;
            bio_s_msg = BIO_new_file(*(++argv), "w");
        }
#ifndef OPENSSL_NO_SSL_TRACE
        else if (strcmp(*argv, "-trace") == 0) {
            s_msg = 2;
        }
#endif
        else if (strcmp(*argv, "-hack") == 0) {
            hack = 1;
        } else if (strcmp(*argv, "-state") == 0) {
            state = 1;
        } else if (strcmp(*argv, "-crlf") == 0) {
            s_crlf = 1;
        } else if (strcmp(*argv, "-quiet") == 0) {
            s_quiet = 1;
        } else if (strcmp(*argv, "-brief") == 0) {
            s_quiet = 1;
            s_brief = 1;
            verify_quiet = 1;
        } else if (strcmp(*argv, "-no_tmp_rsa") == 0) {
            no_tmp_rsa = 1;
        } else if (strcmp(*argv, "-no_dhe") == 0) {
            no_dhe = 1;
        } else if (strcmp(*argv, "-no_ecdhe") == 0) {
            no_ecdhe = 1;
        } else if (strcmp(*argv, "-no_resume_ephemeral") == 0) {
            no_resume_ephemeral = 1;
        }
#ifndef OPENSSL_NO_PSK
        else if (strcmp(*argv, "-psk_hint") == 0) {
            if (--argc < 1)
                goto bad;
            psk_identity_hint = *(++argv);
        } else if (strcmp(*argv, "-psk") == 0) {
            size_t i;

            if (--argc < 1)
                goto bad;
            psk_key = *(++argv);
            for (i = 0; i < strlen(psk_key); i++) {
                if (isxdigit((unsigned char)psk_key[i]))
                    continue;
                BIO_printf(bio_err, "Not a hex number '%s'\n", *argv);
                goto bad;
            }
        }
#endif
#ifndef OPENSSL_NO_SRP
        else if (strcmp(*argv, "-srpvfile") == 0) {
            if (--argc < 1)
                goto bad;
            srp_verifier_file = *(++argv);
            meth = TLSv1_server_method();
        } else if (strcmp(*argv, "-srpuserseed") == 0) {
            if (--argc < 1)
                goto bad;
            srpuserseed = *(++argv);
            meth = TLSv1_server_method();
        }
#endif
        else if (strcmp(*argv, "-rev") == 0) {
            rev = 1;
        } else if (strcmp(*argv, "-www") == 0) {
            www = 1;
        } else if (strcmp(*argv, "-WWW") == 0) {
            www = 2;
        } else if (strcmp(*argv, "-HTTP") == 0) {
            www = 3;
        }
#ifndef OPENSSL_NO_SSL2
        else if (strcmp(*argv, "-ssl2") == 0) {
            no_ecdhe = 1;
            meth = SSLv2_server_method();
        }
#endif
#ifndef OPENSSL_NO_SSL3_METHOD
        else if (strcmp(*argv, "-ssl3") == 0) {
            meth = SSLv3_server_method();
        }
#endif
#ifndef OPENSSL_NO_TLS1
        else if (strcmp(*argv, "-tls1") == 0) {
            meth = TLSv1_server_method();
        } else if (strcmp(*argv, "-tls1_1") == 0) {
            meth = TLSv1_1_server_method();
        } else if (strcmp(*argv, "-tls1_2") == 0) {
            meth = TLSv1_2_server_method();
        }
#endif
#ifndef OPENSSL_NO_DTLS1
        else if (strcmp(*argv, "-dtls") == 0) {
            meth = DTLS_server_method();
            socket_type = SOCK_DGRAM;
        } else if (strcmp(*argv, "-dtls1") == 0) {
            meth = DTLSv1_server_method();
            socket_type = SOCK_DGRAM;
        } else if (strcmp(*argv, "-dtls1_2") == 0) {
            meth = DTLSv1_2_server_method();
            socket_type = SOCK_DGRAM;
        } else if (strcmp(*argv, "-timeout") == 0)
            enable_timeouts = 1;
        else if (strcmp(*argv, "-mtu") == 0) {
            if (--argc < 1)
                goto bad;
            socket_mtu = atol(*(++argv));
        } else if (strcmp(*argv, "-chain") == 0)
            cert_chain = 1;
#endif
        else if (strcmp(*argv, "-id_prefix") == 0) {
            if (--argc < 1)
                goto bad;
            session_id_prefix = *(++argv);
        }
#ifndef OPENSSL_NO_ENGINE
        else if (strcmp(*argv, "-engine") == 0) {
            if (--argc < 1)
                goto bad;
            engine_id = *(++argv);
        }
#endif
        else if (strcmp(*argv, "-rand") == 0) {
            if (--argc < 1)
                goto bad;
            inrand = *(++argv);
        }
#ifndef OPENSSL_NO_TLSEXT
        else if (strcmp(*argv, "-servername") == 0) {
            if (--argc < 1)
                goto bad;
            tlsextcbp.servername = *(++argv);
        } else if (strcmp(*argv, "-servername_fatal") == 0) {
            tlsextcbp.extension_error = SSL_TLSEXT_ERR_ALERT_FATAL;
        } else if (strcmp(*argv, "-cert2") == 0) {
            if (--argc < 1)
                goto bad;
            s_cert_file2 = *(++argv);
        } else if (strcmp(*argv, "-key2") == 0) {
            if (--argc < 1)
                goto bad;
            s_key_file2 = *(++argv);
        }
# ifndef OPENSSL_NO_NEXTPROTONEG
        else if (strcmp(*argv, "-nextprotoneg") == 0) {
            if (--argc < 1)
                goto bad;
            next_proto_neg_in = *(++argv);
        }
# endif
        else if (strcmp(*argv, "-alpn") == 0) {
            if (--argc < 1)
                goto bad;
            alpn_in = *(++argv);
        }
#endif
#if !defined(OPENSSL_NO_JPAKE) && !defined(OPENSSL_NO_PSK)
        else if (strcmp(*argv, "-jpake") == 0) {
            if (--argc < 1)
                goto bad;
            jpake_secret = *(++argv);
        }
#endif
#ifndef OPENSSL_NO_SRTP
        else if (strcmp(*argv, "-use_srtp") == 0) {
            if (--argc < 1)
                goto bad;
            srtp_profiles = *(++argv);
        }
#endif
        else if (strcmp(*argv, "-keymatexport") == 0) {
            if (--argc < 1)
                goto bad;
            keymatexportlabel = *(++argv);
        } else if (strcmp(*argv, "-keymatexportlen") == 0) {
            if (--argc < 1)
                goto bad;
            keymatexportlen = atoi(*(++argv));
            if (keymatexportlen == 0)
                goto bad;
        } else {
            BIO_printf(bio_err, "unknown option %s\n", *argv);
            badop = 1;
            break;
        }
        argc--;
        argv++;
    }
    if (badop) {
 bad:
        sv_usage();
        goto end;
    }
#ifndef OPENSSL_NO_DTLS1
    if (www && socket_type == SOCK_DGRAM) {
        BIO_printf(bio_err, "Can't use -HTTP, -www or -WWW with DTLS\n");
        goto end;
    }
#endif

#if !defined(OPENSSL_NO_JPAKE) && !defined(OPENSSL_NO_PSK)
    if (jpake_secret) {
        if (psk_key) {
            BIO_printf(bio_err, "Can't use JPAKE and PSK together\n");
            goto end;
        }
        psk_identity = "JPAKE";
    }
#endif

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

#ifndef OPENSSL_NO_ENGINE
    e = setup_engine(bio_err, engine_id, 1);
#endif

    if (!app_passwd(bio_err, passarg, dpassarg, &pass, &dpass)) {
        BIO_printf(bio_err, "Error getting password\n");
        goto end;
    }

    if (s_key_file == NULL)
        s_key_file = s_cert_file;
#ifndef OPENSSL_NO_TLSEXT
    if (s_key_file2 == NULL)
        s_key_file2 = s_cert_file2;
#endif

    if (!load_excert(&exc, bio_err))
        goto end;

    if (nocert == 0) {
        s_key = load_key(bio_err, s_key_file, s_key_format, 0, pass, e,
                         "server certificate private key file");
        if (!s_key) {
            ERR_print_errors(bio_err);
            goto end;
        }

        s_cert = load_cert(bio_err, s_cert_file, s_cert_format,
                           NULL, e, "server certificate file");

        if (!s_cert) {
            ERR_print_errors(bio_err);
            goto end;
        }
        if (s_chain_file) {
            s_chain = load_certs(bio_err, s_chain_file, FORMAT_PEM,
                                 NULL, e, "server certificate chain");
            if (!s_chain)
                goto end;
        }
#ifndef OPENSSL_NO_TLSEXT
        if (tlsextcbp.servername) {
            s_key2 = load_key(bio_err, s_key_file2, s_key_format, 0, pass, e,
                              "second server certificate private key file");
            if (!s_key2) {
                ERR_print_errors(bio_err);
                goto end;
            }

            s_cert2 = load_cert(bio_err, s_cert_file2, s_cert_format,
                                NULL, e, "second server certificate file");

            if (!s_cert2) {
                ERR_print_errors(bio_err);
                goto end;
            }
        }
#endif                          /* OPENSSL_NO_TLSEXT */
    }
#if !defined(OPENSSL_NO_TLSEXT)
# if !defined(OPENSSL_NO_NEXTPROTONEG)
    if (next_proto_neg_in) {
        unsigned short len;
        next_proto.data = next_protos_parse(&len, next_proto_neg_in);
        if (next_proto.data == NULL)
            goto end;
        next_proto.len = len;
    } else {
        next_proto.data = NULL;
    }
# endif
    alpn_ctx.data = NULL;
    if (alpn_in) {
        unsigned short len;
        alpn_ctx.data = next_protos_parse(&len, alpn_in);
        if (alpn_ctx.data == NULL)
            goto end;
        alpn_ctx.len = len;
    }
#endif

    if (crl_file) {
        X509_CRL *crl;
        crl = load_crl(crl_file, crl_format);
        if (!crl) {
            BIO_puts(bio_err, "Error loading CRL\n");
            ERR_print_errors(bio_err);
            goto end;
        }
        crls = sk_X509_CRL_new_null();
        if (!crls || !sk_X509_CRL_push(crls, crl)) {
            BIO_puts(bio_err, "Error adding CRL\n");
            ERR_print_errors(bio_err);
            X509_CRL_free(crl);
            goto end;
        }
    }

    if (s_dcert_file) {

        if (s_dkey_file == NULL)
            s_dkey_file = s_dcert_file;

        s_dkey = load_key(bio_err, s_dkey_file, s_dkey_format,
                          0, dpass, e, "second certificate private key file");
        if (!s_dkey) {
            ERR_print_errors(bio_err);
            goto end;
        }

        s_dcert = load_cert(bio_err, s_dcert_file, s_dcert_format,
                            NULL, e, "second server certificate file");

        if (!s_dcert) {
            ERR_print_errors(bio_err);
            goto end;
        }
        if (s_dchain_file) {
            s_dchain = load_certs(bio_err, s_dchain_file, FORMAT_PEM,
                                  NULL, e, "second server certificate chain");
            if (!s_dchain)
                goto end;
        }

    }

    if (!app_RAND_load_file(NULL, bio_err, 1) && inrand == NULL
        && !RAND_status()) {
        BIO_printf(bio_err,
                   "warning, not much extra random data, consider using the -rand option\n");
    }
    if (inrand != NULL)
        BIO_printf(bio_err, "%ld semi-random bytes loaded\n",
                   app_RAND_load_files(inrand));

    if (bio_s_out == NULL) {
        if (s_quiet && !s_debug) {
            bio_s_out = BIO_new(BIO_s_null());
            if (s_msg && !bio_s_msg)
                bio_s_msg = BIO_new_fp(stdout, BIO_NOCLOSE);
        } else {
            if (bio_s_out == NULL)
                bio_s_out = BIO_new_fp(stdout, BIO_NOCLOSE);
        }
    }
#if !defined(OPENSSL_NO_RSA) || !defined(OPENSSL_NO_DSA) || !defined(OPENSSL_NO_ECDSA)
    if (nocert)
#endif
    {
        s_cert_file = NULL;
        s_key_file = NULL;
        s_dcert_file = NULL;
        s_dkey_file = NULL;
#ifndef OPENSSL_NO_TLSEXT
        s_cert_file2 = NULL;
        s_key_file2 = NULL;
#endif
    }

    ctx = SSL_CTX_new(meth);
    if (ctx == NULL) {
        ERR_print_errors(bio_err);
        goto end;
    }
    if (session_id_prefix) {
        if (strlen(session_id_prefix) >= 32)
            BIO_printf(bio_err,
                       "warning: id_prefix is too long, only one new session will be possible\n");
        else if (strlen(session_id_prefix) >= 16)
            BIO_printf(bio_err,
                       "warning: id_prefix is too long if you use SSLv2\n");
        if (!SSL_CTX_set_generate_session_id(ctx, generate_session_id)) {
            BIO_printf(bio_err, "error setting 'id_prefix'\n");
            ERR_print_errors(bio_err);
            goto end;
        }
        BIO_printf(bio_err, "id_prefix '%s' set.\n", session_id_prefix);
    }
    SSL_CTX_set_quiet_shutdown(ctx, 1);
    if (hack)
        SSL_CTX_set_options(ctx, SSL_OP_NETSCAPE_DEMO_CIPHER_CHANGE_BUG);
    if (exc)
        ssl_ctx_set_excert(ctx, exc);

    if (state)
        SSL_CTX_set_info_callback(ctx, apps_ssl_info_callback);
    if (no_cache)
        SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_OFF);
    else if (ext_cache)
        init_session_cache_ctx(ctx);
    else
        SSL_CTX_sess_set_cache_size(ctx, 128);

#ifndef OPENSSL_NO_SRTP
    if (srtp_profiles != NULL)
        SSL_CTX_set_tlsext_use_srtp(ctx, srtp_profiles);
#endif

#if 0
    if (cipher == NULL)
        cipher = getenv("SSL_CIPHER");
#endif

#if 0
    if (s_cert_file == NULL) {
        BIO_printf(bio_err,
                   "You must specify a certificate file for the server to use\n");
        goto end;
    }
#endif

    if ((!SSL_CTX_load_verify_locations(ctx, CAfile, CApath)) ||
        (!SSL_CTX_set_default_verify_paths(ctx))) {
        /* BIO_printf(bio_err,"X509_load_verify_locations\n"); */
        ERR_print_errors(bio_err);
        /* goto end; */
    }
    if (vpm)
        SSL_CTX_set1_param(ctx, vpm);

    ssl_ctx_add_crls(ctx, crls, 0);

    if (!args_ssl_call(ctx, bio_err, cctx, ssl_args, no_ecdhe, no_jpake))
        goto end;

    if (!ssl_load_stores(ctx, vfyCApath, vfyCAfile, chCApath, chCAfile,
                         crls, crl_download)) {
        BIO_printf(bio_err, "Error loading store locations\n");
        ERR_print_errors(bio_err);
        goto end;
    }
#ifndef OPENSSL_NO_TLSEXT
    if (s_cert2) {
        ctx2 = SSL_CTX_new(meth);
        if (ctx2 == NULL) {
            ERR_print_errors(bio_err);
            goto end;
        }
    }

    if (ctx2) {
        BIO_printf(bio_s_out, "Setting secondary ctx parameters\n");

        if (session_id_prefix) {
            if (strlen(session_id_prefix) >= 32)
                BIO_printf(bio_err,
                           "warning: id_prefix is too long, only one new session will be possible\n");
            else if (strlen(session_id_prefix) >= 16)
                BIO_printf(bio_err,
                           "warning: id_prefix is too long if you use SSLv2\n");
            if (!SSL_CTX_set_generate_session_id(ctx2, generate_session_id)) {
                BIO_printf(bio_err, "error setting 'id_prefix'\n");
                ERR_print_errors(bio_err);
                goto end;
            }
            BIO_printf(bio_err, "id_prefix '%s' set.\n", session_id_prefix);
        }
        SSL_CTX_set_quiet_shutdown(ctx2, 1);
        if (hack)
            SSL_CTX_set_options(ctx2, SSL_OP_NETSCAPE_DEMO_CIPHER_CHANGE_BUG);
        if (exc)
            ssl_ctx_set_excert(ctx2, exc);

        if (state)
            SSL_CTX_set_info_callback(ctx2, apps_ssl_info_callback);

        if (no_cache)
            SSL_CTX_set_session_cache_mode(ctx2, SSL_SESS_CACHE_OFF);
        else if (ext_cache)
            init_session_cache_ctx(ctx2);
        else
            SSL_CTX_sess_set_cache_size(ctx2, 128);

        if ((!SSL_CTX_load_verify_locations(ctx2, CAfile, CApath)) ||
            (!SSL_CTX_set_default_verify_paths(ctx2))) {
            ERR_print_errors(bio_err);
        }
        if (vpm)
            SSL_CTX_set1_param(ctx2, vpm);

        ssl_ctx_add_crls(ctx2, crls, 0);

        if (!args_ssl_call(ctx2, bio_err, cctx, ssl_args, no_ecdhe, no_jpake))
            goto end;

    }
# ifndef OPENSSL_NO_NEXTPROTONEG
    if (next_proto.data)
        SSL_CTX_set_next_protos_advertised_cb(ctx, next_proto_cb,
                                              &next_proto);
# endif
    if (alpn_ctx.data)
        SSL_CTX_set_alpn_select_cb(ctx, alpn_cb, &alpn_ctx);
#endif

#ifndef OPENSSL_NO_DH
    if (!no_dhe) {
        DH *dh = NULL;

        if (dhfile)
            dh = load_dh_param(dhfile);
        else if (s_cert_file)
            dh = load_dh_param(s_cert_file);

        if (dh != NULL) {
            BIO_printf(bio_s_out, "Setting temp DH parameters\n");
        } else {
            BIO_printf(bio_s_out, "Using default temp DH parameters\n");
            dh = get_dh2048();
            if (dh == NULL) {
                ERR_print_errors(bio_err);
                goto end;
            }
        }
        (void)BIO_flush(bio_s_out);

        SSL_CTX_set_tmp_dh(ctx, dh);
# ifndef OPENSSL_NO_TLSEXT
        if (ctx2) {
            if (!dhfile) {
                DH *dh2 = load_dh_param(s_cert_file2);
                if (dh2 != NULL) {
                    BIO_printf(bio_s_out, "Setting temp DH parameters\n");
                    (void)BIO_flush(bio_s_out);

                    DH_free(dh);
                    dh = dh2;
                }
            }
            SSL_CTX_set_tmp_dh(ctx2, dh);
        }
# endif
        DH_free(dh);
    }
#endif

    if (!set_cert_key_stuff(ctx, s_cert, s_key, s_chain, build_chain))
        goto end;
#ifndef OPENSSL_NO_TLSEXT
    if (s_serverinfo_file != NULL
        && !SSL_CTX_use_serverinfo_file(ctx, s_serverinfo_file)) {
        ERR_print_errors(bio_err);
        goto end;
    }
#endif
#ifndef OPENSSL_NO_TLSEXT
    if (ctx2 && !set_cert_key_stuff(ctx2, s_cert2, s_key2, NULL, build_chain))
        goto end;
#endif
    if (s_dcert != NULL) {
        if (!set_cert_key_stuff(ctx, s_dcert, s_dkey, s_dchain, build_chain))
            goto end;
    }
#ifndef OPENSSL_NO_RSA
# if 1
    if (!no_tmp_rsa) {
        SSL_CTX_set_tmp_rsa_callback(ctx, tmp_rsa_cb);
#  ifndef OPENSSL_NO_TLSEXT
        if (ctx2)
            SSL_CTX_set_tmp_rsa_callback(ctx2, tmp_rsa_cb);
#  endif
    }
# else
    if (!no_tmp_rsa && SSL_CTX_need_tmp_RSA(ctx)) {
        RSA *rsa;

        BIO_printf(bio_s_out, "Generating temp (512 bit) RSA key...");
        BIO_flush(bio_s_out);

        rsa = RSA_generate_key(512, RSA_F4, NULL);

        if (!SSL_CTX_set_tmp_rsa(ctx, rsa)) {
            ERR_print_errors(bio_err);
            goto end;
        }
#  ifndef OPENSSL_NO_TLSEXT
        if (ctx2) {
            if (!SSL_CTX_set_tmp_rsa(ctx2, rsa)) {
                ERR_print_errors(bio_err);
                goto end;
            }
        }
#  endif
        RSA_free(rsa);
        BIO_printf(bio_s_out, "\n");
    }
# endif
#endif

#ifndef OPENSSL_NO_PSK
# ifdef OPENSSL_NO_JPAKE
    if (psk_key != NULL)
# else
    if (psk_key != NULL || jpake_secret)
# endif
    {
        if (s_debug)
            BIO_printf(bio_s_out,
                       "PSK key given or JPAKE in use, setting server callback\n");
        SSL_CTX_set_psk_server_callback(ctx, psk_server_cb);
    }

    if (!SSL_CTX_use_psk_identity_hint(ctx, psk_identity_hint)) {
        BIO_printf(bio_err, "error setting PSK identity hint to context\n");
        ERR_print_errors(bio_err);
        goto end;
    }
#endif

    SSL_CTX_set_verify(ctx, s_server_verify, verify_callback);
    SSL_CTX_set_session_id_context(ctx, (void *)&s_server_session_id_context,
                                   sizeof s_server_session_id_context);

    /* Set DTLS cookie generation and verification callbacks */
    SSL_CTX_set_cookie_generate_cb(ctx, generate_cookie_callback);
    SSL_CTX_set_cookie_verify_cb(ctx, verify_cookie_callback);

#ifndef OPENSSL_NO_TLSEXT
    if (ctx2) {
        SSL_CTX_set_verify(ctx2, s_server_verify, verify_callback);
        SSL_CTX_set_session_id_context(ctx2,
                                       (void *)&s_server_session_id_context,
                                       sizeof s_server_session_id_context);

        tlsextcbp.biodebug = bio_s_out;
        SSL_CTX_set_tlsext_servername_callback(ctx2, ssl_servername_cb);
        SSL_CTX_set_tlsext_servername_arg(ctx2, &tlsextcbp);
        SSL_CTX_set_tlsext_servername_callback(ctx, ssl_servername_cb);
        SSL_CTX_set_tlsext_servername_arg(ctx, &tlsextcbp);
    }
#endif

#ifndef OPENSSL_NO_SRP
    if (srp_verifier_file != NULL) {
        srp_callback_parm.vb = SRP_VBASE_new(srpuserseed);
        srp_callback_parm.user = NULL;
        srp_callback_parm.login = NULL;
        if ((ret =
             SRP_VBASE_init(srp_callback_parm.vb,
                            srp_verifier_file)) != SRP_NO_ERROR) {
            BIO_printf(bio_err,
                       "Cannot initialize SRP verifier file \"%s\":ret=%d\n",
                       srp_verifier_file, ret);
            goto end;
        }
        SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, verify_callback);
        SSL_CTX_set_srp_cb_arg(ctx, &srp_callback_parm);
        SSL_CTX_set_srp_username_callback(ctx, ssl_srp_server_param_cb);
    } else
#endif
    if (CAfile != NULL) {
        SSL_CTX_set_client_CA_list(ctx, SSL_load_client_CA_file(CAfile));
#ifndef OPENSSL_NO_TLSEXT
        if (ctx2)
            SSL_CTX_set_client_CA_list(ctx2, SSL_load_client_CA_file(CAfile));
#endif
    }

    BIO_printf(bio_s_out, "ACCEPT\n");
    (void)BIO_flush(bio_s_out);
    if (rev)
        do_server(port, socket_type, &accept_socket, rev_body, context,
                  naccept);
    else if (www)
        do_server(port, socket_type, &accept_socket, www_body, context,
                  naccept);
    else
        do_server(port, socket_type, &accept_socket, sv_body, context,
                  naccept);
    print_stats(bio_s_out, ctx);
    ret = 0;
 end:
    if (ctx != NULL)
        SSL_CTX_free(ctx);
    if (s_cert)
        X509_free(s_cert);
    if (crls)
        sk_X509_CRL_pop_free(crls, X509_CRL_free);
    if (s_dcert)
        X509_free(s_dcert);
    if (s_key)
        EVP_PKEY_free(s_key);
    if (s_dkey)
        EVP_PKEY_free(s_dkey);
    if (s_chain)
        sk_X509_pop_free(s_chain, X509_free);
    if (s_dchain)
        sk_X509_pop_free(s_dchain, X509_free);
    if (pass)
        OPENSSL_free(pass);
    if (dpass)
        OPENSSL_free(dpass);
    if (vpm)
        X509_VERIFY_PARAM_free(vpm);
    free_sessions();
#ifndef OPENSSL_NO_TLSEXT
    if (tlscstatp.host)
        OPENSSL_free(tlscstatp.host);
    if (tlscstatp.port)
        OPENSSL_free(tlscstatp.port);
    if (tlscstatp.path)
        OPENSSL_free(tlscstatp.path);
    if (ctx2 != NULL)
        SSL_CTX_free(ctx2);
    if (s_cert2)
        X509_free(s_cert2);
    if (s_key2)
        EVP_PKEY_free(s_key2);
    if (serverinfo_in != NULL)
        BIO_free(serverinfo_in);
# ifndef OPENSSL_NO_NEXTPROTONEG
    if (next_proto.data)
        OPENSSL_free(next_proto.data);
# endif
    if (alpn_ctx.data)
        OPENSSL_free(alpn_ctx.data);
#endif
    ssl_excert_free(exc);
    if (ssl_args)
        sk_OPENSSL_STRING_free(ssl_args);
    if (cctx)
        SSL_CONF_CTX_free(cctx);
#ifndef OPENSSL_NO_JPAKE
    if (jpake_secret && psk_key)
        OPENSSL_free(psk_key);
#endif
    if (bio_s_out != NULL) {
        BIO_free(bio_s_out);
        bio_s_out = NULL;
    }
    if (bio_s_msg != NULL) {
        BIO_free(bio_s_msg);
        bio_s_msg = NULL;
    }
    apps_shutdown();
    OPENSSL_EXIT(ret);
}
