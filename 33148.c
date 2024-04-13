void ssl_set_sig_mask(unsigned long *pmask_a, SSL *s, int op)
	{
	const unsigned char *sigalgs;
	size_t i, sigalgslen;
	int have_rsa = 0, have_dsa = 0, have_ecdsa = 0;
	/* Now go through all signature algorithms seeing if we support
	 * any for RSA, DSA, ECDSA. Do this for all versions not just
	 * TLS 1.2. To keep down calls to security callback only check
	 * if we have to.
	 */
	sigalgslen = tls12_get_psigalgs(s, &sigalgs);
	for (i = 0; i < sigalgslen; i += 2, sigalgs += 2)
		{
		switch(sigalgs[1])
			{
#ifndef OPENSSL_NO_RSA
		case TLSEXT_signature_rsa:
			if (!have_rsa && tls12_sigalg_allowed(s, op, sigalgs))
				have_rsa = 1;
			break;
#endif
#ifndef OPENSSL_NO_DSA
		case TLSEXT_signature_dsa:
			if (!have_dsa && tls12_sigalg_allowed(s, op, sigalgs))
				have_dsa = 1;
			break;
#endif
#ifndef OPENSSL_NO_ECDSA
		case TLSEXT_signature_ecdsa:
			if (!have_ecdsa && tls12_sigalg_allowed(s, op, sigalgs))
				have_ecdsa = 1;
			break;
#endif
			}
		}
	if (!have_rsa)
		*pmask_a |= SSL_aRSA;
	if (!have_dsa)
		*pmask_a |= SSL_aDSS;
	if (!have_ecdsa)
		*pmask_a |= SSL_aECDSA;
	}
