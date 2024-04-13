X509_ATTRIBUTE *X509_REQ_get_attr(const X509_REQ *req, int loc)
{
    return X509at_get_attr(req->req_info->attributes, loc);
}
