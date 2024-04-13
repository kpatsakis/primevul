static int ssl_security_cert_key(SSL *s, SSL_CTX *ctx, X509 *x, int op)
	{
	int secbits;
	EVP_PKEY *pkey = X509_get_pubkey(x);
	if (pkey)
		{
		secbits = EVP_PKEY_security_bits(pkey);
		EVP_PKEY_free(pkey);
		}
	else
		secbits = -1;
	if (s)
		return ssl_security(s, op, secbits, 0, x);
	else
		return ssl_ctx_security(ctx, op, secbits, 0, x);
	}
