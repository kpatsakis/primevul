int dtls1_handle_timeout(SSL *s)
{
    /* if no timer is expired, don't do anything */
    if (!dtls1_is_timer_expired(s)) {
        return 0;
    }

    dtls1_double_timeout(s);

    if (dtls1_check_timeout_num(s) < 0)
        return -1;

    s->d1->timeout.read_timeouts++;
    if (s->d1->timeout.read_timeouts > DTLS1_TMO_READ_COUNT) {
        s->d1->timeout.read_timeouts = 1;
    }
#ifndef OPENSSL_NO_HEARTBEATS
    if (s->tlsext_hb_pending) {
        s->tlsext_hb_pending = 0;
        return dtls1_heartbeat(s);
    }
#endif

    dtls1_start_timer(s);
    return dtls1_retransmit_buffered_messages(s);
}
