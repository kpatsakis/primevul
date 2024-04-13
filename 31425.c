int X509_REQ_add_extensions(X509_REQ *req, STACK_OF(X509_EXTENSION) *exts)
{
    return X509_REQ_add_extensions_nid(req, exts, NID_ext_req);
}
