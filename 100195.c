void shut_down(int code)
{
    int i;
    int bytes_in = 0;
    int bytes_out = 0;

    in_shutdown = 1;

    proc_cleanup();

    i = 0;
    while (backend_cached && backend_cached[i]) {
        proxy_downserver(backend_cached[i]);
        if (backend_cached[i]->last_result.s) {
            free(backend_cached[i]->last_result.s);
        }
        free(backend_cached[i]);
        i++;
    }
    if (backend_cached) free(backend_cached);
    if (mupdate_h) mupdate_disconnect(&mupdate_h);

    if (idling)
        idle_stop(index_mboxname(imapd_index));

    if (imapd_index) index_close(&imapd_index);

    sync_log_done();
    seen_done();
    mboxkey_done();
    mboxlist_close();
    mboxlist_done();

    quotadb_close();
    quotadb_done();

    denydb_close();
    denydb_done();

    annotatemore_close();
    annotate_done();

    idle_done();

    if (config_getswitch(IMAPOPT_STATUSCACHE)) {
        statuscache_close();
        statuscache_done();
    }

    partlist_local_done();

    if (imapd_in) {
        /* Flush the incoming buffer */
        prot_NONBLOCK(imapd_in);
        prot_fill(imapd_in);
        bytes_in = prot_bytes_in(imapd_in);
        prot_free(imapd_in);
    }

    if (imapd_out) {
        /* Flush the outgoing buffer */
        prot_flush(imapd_out);
        bytes_out = prot_bytes_out(imapd_out);
        prot_free(imapd_out);

        /* one less active connection */
        snmp_increment(ACTIVE_CONNECTIONS, -1);
    }

    if (config_auditlog)
        syslog(LOG_NOTICE, "auditlog: traffic sessionid=<%s> bytes_in=<%d> bytes_out=<%d>",
                           session_id(), bytes_in, bytes_out);

    if (protin) protgroup_free(protin);

#ifdef HAVE_SSL
    tls_shutdown_serverengine();
#endif

    cyrus_done();

    exit(code);
}
