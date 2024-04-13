void DTLS_RECORD_LAYER_clear(RECORD_LAYER *rl)
{
    DTLS_RECORD_LAYER *d;
    pitem *item = NULL;
    DTLS1_RECORD_DATA *rdata;
    pqueue *unprocessed_rcds;
    pqueue *processed_rcds;
    pqueue *buffered_app_data;

    d = rl->d;

    while ((item = pqueue_pop(d->unprocessed_rcds.q)) != NULL) {
        rdata = (DTLS1_RECORD_DATA *)item->data;
        OPENSSL_free(rdata->rbuf.buf);
        OPENSSL_free(item->data);
        pitem_free(item);
    }

    while ((item = pqueue_pop(d->processed_rcds.q)) != NULL) {
        rdata = (DTLS1_RECORD_DATA *)item->data;
        OPENSSL_free(rdata->rbuf.buf);
        OPENSSL_free(item->data);
        pitem_free(item);
    }

    while ((item = pqueue_pop(d->buffered_app_data.q)) != NULL) {
        rdata = (DTLS1_RECORD_DATA *)item->data;
        OPENSSL_free(rdata->rbuf.buf);
        OPENSSL_free(item->data);
        pitem_free(item);
    }

    unprocessed_rcds = d->unprocessed_rcds.q;
    processed_rcds = d->processed_rcds.q;
    buffered_app_data = d->buffered_app_data.q;
    memset(d, 0, sizeof(*d));
    d->unprocessed_rcds.q = unprocessed_rcds;
    d->processed_rcds.q = processed_rcds;
    d->buffered_app_data.q = buffered_app_data;
}
