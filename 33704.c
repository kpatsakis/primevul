void SSL3_RECORD_clear(SSL3_RECORD *r, unsigned int num_recs)
{
    unsigned char *comp;
    unsigned int i;

    for (i = 0; i < num_recs; i++) {
        comp = r[i].comp;

        memset(&r[i], 0, sizeof(*r));
        r[i].comp = comp;
    }
}
