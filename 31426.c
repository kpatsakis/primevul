int X509_REQ_add_extensions_nid(X509_REQ *req, STACK_OF(X509_EXTENSION) *exts,
                                int nid)
{
    ASN1_TYPE *at = NULL;
    X509_ATTRIBUTE *attr = NULL;
    if (!(at = ASN1_TYPE_new()) || !(at->value.sequence = ASN1_STRING_new()))
        goto err;

    at->type = V_ASN1_SEQUENCE;
    /* Generate encoding of extensions */
    at->value.sequence->length =
        ASN1_item_i2d((ASN1_VALUE *)exts,
                      &at->value.sequence->data,
                      ASN1_ITEM_rptr(X509_EXTENSIONS));
    if (!(attr = X509_ATTRIBUTE_new()))
        goto err;
    if (!(attr->value.set = sk_ASN1_TYPE_new_null()))
        goto err;
    if (!sk_ASN1_TYPE_push(attr->value.set, at))
        goto err;
    at = NULL;
    attr->single = 0;
    attr->object = OBJ_nid2obj(nid);
    if (!req->req_info->attributes) {
        if (!(req->req_info->attributes = sk_X509_ATTRIBUTE_new_null()))
            goto err;
    }
    if (!sk_X509_ATTRIBUTE_push(req->req_info->attributes, attr))
        goto err;
    return 1;
 err:
    X509_ATTRIBUTE_free(attr);
    ASN1_TYPE_free(at);
    return 0;
}
