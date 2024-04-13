static void tls1_lookup_sigalg(int *phash_nid, int *psign_nid,
                               int *psignhash_nid, const unsigned char *data)
{
    int sign_nid = 0, hash_nid = 0;
    if (!phash_nid && !psign_nid && !psignhash_nid)
        return;
    if (phash_nid || psignhash_nid) {
        hash_nid = tls12_find_nid(data[0], tls12_md,
                                  sizeof(tls12_md) / sizeof(tls12_lookup));
        if (phash_nid)
            *phash_nid = hash_nid;
    }
    if (psign_nid || psignhash_nid) {
        sign_nid = tls12_find_nid(data[1], tls12_sig,
                                  sizeof(tls12_sig) / sizeof(tls12_lookup));
        if (psign_nid)
            *psign_nid = sign_nid;
    }
    if (psignhash_nid) {
        if (sign_nid && hash_nid)
            OBJ_find_sigid_by_algs(psignhash_nid, hash_nid, sign_nid);
        else
            *psignhash_nid = NID_undef;
    }
}
