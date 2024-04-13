int tls12_get_sigid(const EVP_PKEY *pk)
{
    return tls12_find_id(EVP_PKEY_id(pk), tls12_sig, OSSL_NELEM(tls12_sig));
}
