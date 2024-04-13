int tls1_set_sigalgs(CERT *c, const int *psig_nids, size_t salglen, int client)
	{
	unsigned char *sigalgs, *sptr;
	int rhash, rsign;
	size_t i;
	if (salglen & 1)
		return 0;
	sigalgs = OPENSSL_malloc(salglen);
	if (sigalgs == NULL)
		return 0;
	for (i = 0, sptr = sigalgs; i < salglen; i+=2)
		{
		rhash = tls12_find_id(*psig_nids++, tls12_md,
					sizeof(tls12_md)/sizeof(tls12_lookup));
		rsign = tls12_find_id(*psig_nids++, tls12_sig,
				sizeof(tls12_sig)/sizeof(tls12_lookup));

		if (rhash == -1 || rsign == -1)
			goto err;
		*sptr++ = rhash;
		*sptr++ = rsign;
		}

	if (client)
		{
		if (c->client_sigalgs)
			OPENSSL_free(c->client_sigalgs);
		c->client_sigalgs = sigalgs;
		c->client_sigalgslen = salglen;
		}
	else
		{
		if (c->conf_sigalgs)
			OPENSSL_free(c->conf_sigalgs);
		c->conf_sigalgs = sigalgs;
		c->conf_sigalgslen = salglen;
		}

	return 1;

	err:
	OPENSSL_free(sigalgs);
	return 0;
	}
