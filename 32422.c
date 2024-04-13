static int tls_curve_allowed(SSL *s, const unsigned char *curve, int op)
{
    const tls_curve_info *cinfo;
    if (curve[0])
        return 1;
    if ((curve[1] < 1) || ((size_t)curve[1] > OSSL_NELEM(nid_list)))
        return 0;
    cinfo = &nid_list[curve[1] - 1];
# ifdef OPENSSL_NO_EC2M
    if (cinfo->flags & TLS_CURVE_CHAR2)
        return 0;
# endif
    return ssl_security(s, op, cinfo->secbits, cinfo->nid, (void *)curve);
}
