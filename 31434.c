EVP_PKEY *X509_REQ_get_pubkey(X509_REQ *req)
{
    if ((req == NULL) || (req->req_info == NULL))
        return (NULL);
    return (X509_PUBKEY_get(req->req_info->pubkey));
}
