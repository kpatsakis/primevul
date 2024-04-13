int DTLS_RECORD_LAYER_new(RECORD_LAYER *rl)
{
    DTLS_RECORD_LAYER *d;

    if ((d = OPENSSL_malloc(sizeof(*d))) == NULL)
        return (0);

    rl->d = d;

    d->unprocessed_rcds.q = pqueue_new();
    d->processed_rcds.q = pqueue_new();
    d->buffered_app_data.q = pqueue_new();

    if (d->unprocessed_rcds.q == NULL || d->processed_rcds.q == NULL
        || d->buffered_app_data.q == NULL) {
        pqueue_free(d->unprocessed_rcds.q);
        pqueue_free(d->processed_rcds.q);
        pqueue_free(d->buffered_app_data.q);
        OPENSSL_free(d);
        rl->d = NULL;
        return (0);
    }

    return 1;
}
