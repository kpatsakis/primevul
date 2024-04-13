unsigned long ssl3_output_cert_chain(SSL *s, X509 *x)
	{
	unsigned char *p;
	int i;
	unsigned long l=7;
	BUF_MEM *buf;
	int no_chain;

	if ((s->mode & SSL_MODE_NO_AUTO_CHAIN) || s->ctx->extra_certs)
		no_chain = 1;
	else
		no_chain = 0;

	/* TLSv1 sends a chain with nothing in it, instead of an alert */
	buf=s->init_buf;
	if (!BUF_MEM_grow_clean(buf,10))
		{
		SSLerr(SSL_F_SSL3_OUTPUT_CERT_CHAIN,ERR_R_BUF_LIB);
		return(0);
		}
	if (x != NULL)
		{
		if (no_chain)
			{
			if (ssl3_add_cert_to_buf(buf, &l, x))
				return(0);
			}
		else
			{
			X509_STORE_CTX xs_ctx;

			if (!X509_STORE_CTX_init(&xs_ctx,s->ctx->cert_store,x,NULL))
				{
				SSLerr(SSL_F_SSL3_OUTPUT_CERT_CHAIN,ERR_R_X509_LIB);
				return(0);
				}
			X509_verify_cert(&xs_ctx);
			/* Don't leave errors in the queue */
			ERR_clear_error();
			for (i=0; i < sk_X509_num(xs_ctx.chain); i++)
				{
				x = sk_X509_value(xs_ctx.chain, i);

				if (ssl3_add_cert_to_buf(buf, &l, x))
					{
					X509_STORE_CTX_cleanup(&xs_ctx);
					return 0;
					}
				}
			X509_STORE_CTX_cleanup(&xs_ctx);
			}
		}
	/* Thawte special :-) */
	for (i=0; i<sk_X509_num(s->ctx->extra_certs); i++)
		{
		x=sk_X509_value(s->ctx->extra_certs,i);
		if (ssl3_add_cert_to_buf(buf, &l, x))
			return(0);
		}

	l-=7;
	p=(unsigned char *)&(buf->data[4]);
	l2n3(l,p);
	l+=3;
	p=(unsigned char *)&(buf->data[0]);
	*(p++)=SSL3_MT_CERTIFICATE;
	l2n3(l,p);
	l+=4;
	return(l);
	}
