static const tls12_hash_info *tls12_get_hash_info(unsigned char hash_alg)
{
    unsigned int i;
    if (hash_alg == 0)
        return NULL;

    for (i = 0; i < OSSL_NELEM(tls12_md_info); i++) {
        if (tls12_md_info[i].tlsext_hash == hash_alg)
            return tls12_md_info + i;
    }

    return NULL;
}
