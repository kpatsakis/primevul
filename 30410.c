static DH *get_server_static_dh_key(SESS_CERT *scert)
{
    DH *dh_srvr = NULL;
    EVP_PKEY *spkey = NULL;
    int idx = scert->peer_cert_type;

    if (idx >= 0)
        spkey = X509_get_pubkey(scert->peer_pkeys[idx].x509);
    if (spkey) {
        dh_srvr = EVP_PKEY_get1_DH(spkey);
        EVP_PKEY_free(spkey);
    }
    if (dh_srvr == NULL)
        SSLerr(SSL_F_GET_SERVER_STATIC_DH_KEY, ERR_R_INTERNAL_ERROR);
    return dh_srvr;
}
