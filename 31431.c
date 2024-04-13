int X509_REQ_get_attr_count(const X509_REQ *req)
{
    return X509at_get_attr_count(req->req_info->attributes);
}
