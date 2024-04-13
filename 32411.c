static void tls1_lookup_sigalg(int *phash_nid, int *psign_nid,
                               int *psignhash_nid, const unsigned char *data)
{
    int sign_nid = NID_undef, hash_nid = NID_undef;
    if (!phash_nid && !psign_nid && !psignhash_nid)
        return;
    if (phash_nid || psignhash_nid) {
        hash_nid = tls12_find_nid(data[0], tls12_md, OSSL_NELEM(tls12_md));
        if (phash_nid)
            *phash_nid = hash_nid;
    }
    if (psign_nid || psignhash_nid) {
        sign_nid = tls12_find_nid(data[1], tls12_sig, OSSL_NELEM(tls12_sig));
        if (psign_nid)
            *psign_nid = sign_nid;
    }
    if (psignhash_nid) {
        if (sign_nid == NID_undef || hash_nid == NID_undef
            || OBJ_find_sigid_by_algs(psignhash_nid, hash_nid, sign_nid) <= 0)
            *psignhash_nid = NID_undef;
    }
}
