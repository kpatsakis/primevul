int X509_REQ_get_attr_by_OBJ(const X509_REQ *req, ASN1_OBJECT *obj,
                             int lastpos)
{
    return X509at_get_attr_by_OBJ(req->req_info->attributes, obj, lastpos);
}
