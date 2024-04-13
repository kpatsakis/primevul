void DTLS_RECORD_LAYER_free(RECORD_LAYER *rl)
{
    DTLS_RECORD_LAYER_clear(rl);
    pqueue_free(rl->d->unprocessed_rcds.q);
    pqueue_free(rl->d->processed_rcds.q);
    pqueue_free(rl->d->buffered_app_data.q);
    OPENSSL_free(rl->d);
    rl->d = NULL;
}
