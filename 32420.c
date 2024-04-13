int tls1_shared_curve(SSL *s, int nmatch)
{
    const unsigned char *pref, *supp;
    size_t num_pref, num_supp, i, j;
    int k;
    /* Can't do anything on client side */
    if (s->server == 0)
        return -1;
    if (nmatch == -2) {
        if (tls1_suiteb(s)) {
            /*
             * For Suite B ciphersuite determines curve: we already know
             * these are acceptable due to previous checks.
             */
            unsigned long cid = s->s3->tmp.new_cipher->id;
            if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256)
                return NID_X9_62_prime256v1; /* P-256 */
            if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384)
                return NID_secp384r1; /* P-384 */
            /* Should never happen */
            return NID_undef;
        }
        /* If not Suite B just return first preference shared curve */
        nmatch = 0;
    }
    /*
     * Avoid truncation. tls1_get_curvelist takes an int
     * but s->options is a long...
     */
    if (!tls1_get_curvelist
        (s, (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE) != 0, &supp,
         &num_supp))
        /* In practice, NID_undef == 0 but let's be precise. */
        return nmatch == -1 ? 0 : NID_undef;
    if (!tls1_get_curvelist
        (s, !(s->options & SSL_OP_CIPHER_SERVER_PREFERENCE), &pref, &num_pref))
        return nmatch == -1 ? 0 : NID_undef;

    /*
     * If the client didn't send the elliptic_curves extension all of them
     * are allowed.
     */
    if (num_supp == 0 && (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE) != 0) {
        supp = eccurves_all;
        num_supp = sizeof(eccurves_all) / 2;
    } else if (num_pref == 0 &&
               (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE) == 0) {
        pref = eccurves_all;
        num_pref = sizeof(eccurves_all) / 2;
    }

    k = 0;
    for (i = 0; i < num_pref; i++, pref += 2) {
        const unsigned char *tsupp = supp;
        for (j = 0; j < num_supp; j++, tsupp += 2) {
            if (pref[0] == tsupp[0] && pref[1] == tsupp[1]) {
                if (!tls_curve_allowed(s, pref, SSL_SECOP_CURVE_SHARED))
                    continue;
                if (nmatch == k) {
                    int id = (pref[0] << 8) | pref[1];
                    return tls1_ec_curve_id2nid(id, NULL);
                }
                k++;
            }
        }
    }
    if (nmatch == -1)
        return k;
    /* Out of range (nmatch > k). */
    return NID_undef;
}
