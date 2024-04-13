int dtls1_buffer_record(SSL *s, record_pqueue *queue, unsigned char *priority)
{
    DTLS1_RECORD_DATA *rdata;
    pitem *item;

    /* Limit the size of the queue to prevent DOS attacks */
    if (pqueue_size(queue->q) >= 100)
        return 0;

    rdata = OPENSSL_malloc(sizeof(*rdata));
    item = pitem_new(priority, rdata);
    if (rdata == NULL || item == NULL) {
        OPENSSL_free(rdata);
        pitem_free(item);
        SSLerr(SSL_F_DTLS1_BUFFER_RECORD, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    rdata->packet = s->rlayer.packet;
    rdata->packet_length = s->rlayer.packet_length;
    memcpy(&(rdata->rbuf), &s->rlayer.rbuf, sizeof(SSL3_BUFFER));
    memcpy(&(rdata->rrec), &s->rlayer.rrec, sizeof(SSL3_RECORD));

    item->data = rdata;

#ifndef OPENSSL_NO_SCTP
    /* Store bio_dgram_sctp_rcvinfo struct */
    if (BIO_dgram_is_sctp(SSL_get_rbio(s)) &&
        (SSL_get_state(s) == TLS_ST_SR_FINISHED
         || SSL_get_state(s) == TLS_ST_CR_FINISHED)) {
        BIO_ctrl(SSL_get_rbio(s), BIO_CTRL_DGRAM_SCTP_GET_RCVINFO,
                 sizeof(rdata->recordinfo), &rdata->recordinfo);
    }
#endif

    s->rlayer.packet = NULL;
    s->rlayer.packet_length = 0;
    memset(&s->rlayer.rbuf, 0, sizeof(s->rlayer.rbuf));
    memset(&s->rlayer.rrec, 0, sizeof(s->rlayer.rrec));

    if (!ssl3_setup_buffers(s)) {
        SSLerr(SSL_F_DTLS1_BUFFER_RECORD, ERR_R_INTERNAL_ERROR);
        OPENSSL_free(rdata->rbuf.buf);
        OPENSSL_free(rdata);
        pitem_free(item);
        return (-1);
    }

    /* insert should not fail, since duplicates are dropped */
    if (pqueue_insert(queue->q, item) == NULL) {
        SSLerr(SSL_F_DTLS1_BUFFER_RECORD, ERR_R_INTERNAL_ERROR);
        OPENSSL_free(rdata->rbuf.buf);
        OPENSSL_free(rdata);
        pitem_free(item);
        return (-1);
    }

    return (1);
}
