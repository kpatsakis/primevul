int X509_REQ_add1_attr(X509_REQ *req, X509_ATTRIBUTE *attr)
{
    if (X509at_add1_attr(&req->req_info->attributes, attr))
        return 1;
    return 0;
}
