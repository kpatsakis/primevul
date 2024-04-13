static int dtls1_add_cert_to_buf(BUF_MEM *buf, unsigned long *l, X509 *x)
	{
	int n;
	unsigned char *p;

	n=i2d_X509(x,NULL);
	if (!BUF_MEM_grow_clean(buf,(int)(n+(*l)+3)))
		{
		SSLerr(SSL_F_DTLS1_ADD_CERT_TO_BUF,ERR_R_BUF_LIB);
		return 0;
		}
	p=(unsigned char *)&(buf->data[*l]);
	l2n3(n,p);
	i2d_X509(x,&p);
	*l+=n+3;

	return 1;
	}
