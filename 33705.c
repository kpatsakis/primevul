void SSL3_RECORD_release(SSL3_RECORD *r, unsigned int num_recs)
{
    unsigned int i;

    for (i = 0; i < num_recs; i++) {
        OPENSSL_free(r[i].comp);
        r[i].comp = NULL;
    }
}
