int tls12_get_sigid(const EVP_PKEY *pk)
{
    return tls12_find_id(pk->type, tls12_sig,
                         sizeof(tls12_sig) / sizeof(tls12_lookup));
}
