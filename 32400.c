static int tls12_shared_sigalgs(SSL *s, TLS_SIGALGS *shsig,
                                const unsigned char *pref, size_t preflen,
                                const unsigned char *allow, size_t allowlen)
{
    const unsigned char *ptmp, *atmp;
    size_t i, j, nmatch = 0;
    for (i = 0, ptmp = pref; i < preflen; i += 2, ptmp += 2) {
        /* Skip disabled hashes or signature algorithms */
        if (!tls12_sigalg_allowed(s, SSL_SECOP_SIGALG_SHARED, ptmp))
            continue;
        for (j = 0, atmp = allow; j < allowlen; j += 2, atmp += 2) {
            if (ptmp[0] == atmp[0] && ptmp[1] == atmp[1]) {
                nmatch++;
                if (shsig) {
                    shsig->rhash = ptmp[0];
                    shsig->rsign = ptmp[1];
                    tls1_lookup_sigalg(&shsig->hash_nid,
                                       &shsig->sign_nid,
                                       &shsig->signandhash_nid, ptmp);
                    shsig++;
                }
                break;
            }
        }
    }
    return nmatch;
}
