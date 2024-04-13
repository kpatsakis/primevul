STACK_OF(X509_EXTENSION) *X509_REQ_get_extensions(X509_REQ *req)
{
    X509_ATTRIBUTE *attr;
    ASN1_TYPE *ext = NULL;
    int idx, *pnid;
    const unsigned char *p;

    if ((req == NULL) || (req->req_info == NULL) || !ext_nids)
        return (NULL);
    for (pnid = ext_nids; *pnid != NID_undef; pnid++) {
        idx = X509_REQ_get_attr_by_NID(req, *pnid, -1);
        if (idx == -1)
            continue;
        attr = X509_REQ_get_attr(req, idx);
        if (attr->single)
            ext = attr->value.single;
        else if (sk_ASN1_TYPE_num(attr->value.set))
            ext = sk_ASN1_TYPE_value(attr->value.set, 0);
        break;
    }
    if (!ext || (ext->type != V_ASN1_SEQUENCE))
        return NULL;
    p = ext->value.sequence->data;
    return (STACK_OF(X509_EXTENSION) *)
        ASN1_item_d2i(NULL, &p, ext->value.sequence->length,
                      ASN1_ITEM_rptr(X509_EXTENSIONS));
}
