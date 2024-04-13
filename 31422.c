int X509_REQ_add1_attr_by_NID(X509_REQ *req,
                              int nid, int type,
                              const unsigned char *bytes, int len)
{
    if (X509at_add1_attr_by_NID(&req->req_info->attributes, nid,
                                type, bytes, len))
        return 1;
    return 0;
}
