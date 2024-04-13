static int tls_curve_allowed(SSL *s, const unsigned char *curve, int op)
	{
	tls_curve_info *cinfo;
	if (curve[0])
		return 1;
	if ((curve[1] < 1) || ((size_t)curve[1] >
				sizeof(nid_list)/sizeof(nid_list[0])))
		return 0;
	cinfo = &nid_list[curve[1]-1];
	return ssl_security(s, op, cinfo->secbits, cinfo->nid, (void *)curve);
	}
