int X509_REQ_add1_attr_by_txt(X509_REQ *req,
                              const char *attrname, int type,
                              const unsigned char *bytes, int len)
{
    if (X509at_add1_attr_by_txt(&req->req_info->attributes, attrname,
                                type, bytes, len))
        return 1;
    return 0;
}
