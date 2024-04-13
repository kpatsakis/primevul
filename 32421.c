int tls_check_serverhello_tlsext_early(SSL *s, const PACKET *ext,
                                       const PACKET *session_id,
                                       SSL_SESSION **ret)
{
    unsigned int i;
    PACKET local_ext = *ext;
    int retv = -1;

    int have_ticket = 0;
    int use_ticket = tls_use_ticket(s);

    *ret = NULL;
    s->tlsext_ticket_expected = 0;
    s->s3->flags &= ~TLS1_FLAGS_RECEIVED_EXTMS;

    /*
     * If tickets disabled behave as if no ticket present to permit stateful
     * resumption.
     */
    if ((s->version <= SSL3_VERSION))
        return 0;

    if (!PACKET_get_net_2(&local_ext, &i)) {
        retv = 0;
        goto end;
    }
    while (PACKET_remaining(&local_ext) >= 4) {
        unsigned int type, size;

        if (!PACKET_get_net_2(&local_ext, &type)
            || !PACKET_get_net_2(&local_ext, &size)) {
            /* Shouldn't ever happen */
            retv = -1;
            goto end;
        }
        if (PACKET_remaining(&local_ext) < size) {
            retv = 0;
            goto end;
        }
        if (type == TLSEXT_TYPE_session_ticket && use_ticket) {
            int r;
            const unsigned char *etick;

            /* Duplicate extension */
            if (have_ticket != 0) {
                retv = -1;
                goto end;
            }
            have_ticket = 1;

            if (size == 0) {
                /*
                 * The client will accept a ticket but doesn't currently have
                 * one.
                 */
                s->tlsext_ticket_expected = 1;
                retv = 1;
                continue;
            }
            if (s->tls_session_secret_cb) {
                /*
                 * Indicate that the ticket couldn't be decrypted rather than
                 * generating the session from ticket now, trigger
                 * abbreviated handshake based on external mechanism to
                 * calculate the master secret later.
                 */
                retv = 2;
                continue;
            }
            if (!PACKET_get_bytes(&local_ext, &etick, size)) {
                /* Shouldn't ever happen */
                retv = -1;
                goto end;
            }
            r = tls_decrypt_ticket(s, etick, size, PACKET_data(session_id),
                                   PACKET_remaining(session_id), ret);
            switch (r) {
            case 2:            /* ticket couldn't be decrypted */
                s->tlsext_ticket_expected = 1;
                retv = 2;
                break;
            case 3:            /* ticket was decrypted */
                retv = r;
                break;
            case 4:            /* ticket decrypted but need to renew */
                s->tlsext_ticket_expected = 1;
                retv = 3;
                break;
            default:           /* fatal error */
                retv = -1;
                break;
            }
            continue;
        } else {
            if (type == TLSEXT_TYPE_extended_master_secret)
                s->s3->flags |= TLS1_FLAGS_RECEIVED_EXTMS;
            if (!PACKET_forward(&local_ext, size)) {
                retv = -1;
                goto end;
            }
        }
    }
    if (have_ticket == 0)
        retv = 0;
 end:
    return retv;
}
