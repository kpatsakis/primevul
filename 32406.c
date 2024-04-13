int tls1_check_ec_tmp_key(SSL *s, unsigned long cid)
{
    /*
     * If Suite B, AES128 MUST use P-256 and AES256 MUST use P-384, no other
     * curves permitted.
     */
    if (tls1_suiteb(s)) {
        unsigned char curve_id[2];
        /* Curve to check determined by ciphersuite */
        if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256)
            curve_id[1] = TLSEXT_curve_P_256;
        else if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384)
            curve_id[1] = TLSEXT_curve_P_384;
        else
            return 0;
        curve_id[0] = 0;
        /* Check this curve is acceptable */
        if (!tls1_check_ec_key(s, curve_id, NULL))
            return 0;
        return 1;
    }
    /* Need a shared curve */
    if (tls1_shared_curve(s, 0))
        return 1;
    return 0;
}
