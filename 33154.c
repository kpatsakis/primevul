int tls1_check_chain(SSL *s, X509 *x, EVP_PKEY *pk, STACK_OF(X509) *chain,
									int idx)
	{
	int i;
	int rv = 0;
	int check_flags = 0, strict_mode;
	CERT_PKEY *cpk = NULL;
	CERT *c = s->cert;
	unsigned int suiteb_flags = tls1_suiteb(s);
	/* idx == -1 means checking server chains */
	if (idx != -1)
		{
		/* idx == -2 means checking client certificate chains */
		if (idx == -2)
			{
			cpk = c->key;
			idx = cpk - c->pkeys;
			}
		else
			cpk = c->pkeys + idx;
		x = cpk->x509;
		pk = cpk->privatekey;
		chain = cpk->chain;
		strict_mode = c->cert_flags & SSL_CERT_FLAGS_CHECK_TLS_STRICT;
		/* If no cert or key, forget it */
		if (!x || !pk)
			goto end;
#ifdef OPENSSL_SSL_DEBUG_BROKEN_PROTOCOL
		/* Allow any certificate to pass test */
		if (s->cert->cert_flags & SSL_CERT_FLAG_BROKEN_PROTOCOL)
			{
			rv = CERT_PKEY_STRICT_FLAGS|CERT_PKEY_EXPLICIT_SIGN|CERT_PKEY_VALID|CERT_PKEY_SIGN;
			cpk->valid_flags = rv;
			return rv;
			}
#endif
		}
	else
		{
		if (!x || !pk)
			goto end;
		idx = ssl_cert_type(x, pk);
		if (idx == -1)
			goto end;
		cpk = c->pkeys + idx;
		if (c->cert_flags & SSL_CERT_FLAGS_CHECK_TLS_STRICT)
			check_flags = CERT_PKEY_STRICT_FLAGS;
		else
			check_flags = CERT_PKEY_VALID_FLAGS;
		strict_mode = 1;
		}

	if (suiteb_flags)
		{
		int ok;
		if (check_flags)
			check_flags |= CERT_PKEY_SUITEB;
		ok = X509_chain_check_suiteb(NULL, x, chain, suiteb_flags);
		if (ok != X509_V_OK)
			{
			if (check_flags)
				rv |= CERT_PKEY_SUITEB;
			else
				goto end;
			}
		}

	/* Check all signature algorithms are consistent with
	 * signature algorithms extension if TLS 1.2 or later
	 * and strict mode.
	 */
	if (TLS1_get_version(s) >= TLS1_2_VERSION && strict_mode)
		{
		int default_nid;
		unsigned char rsign = 0;
		if (c->peer_sigalgs)
			default_nid = 0;
		/* If no sigalgs extension use defaults from RFC5246 */
		else
			{
			switch(idx)
				{	
			case SSL_PKEY_RSA_ENC:
			case SSL_PKEY_RSA_SIGN:
			case SSL_PKEY_DH_RSA:
				rsign = TLSEXT_signature_rsa;
				default_nid = NID_sha1WithRSAEncryption;
				break;

			case SSL_PKEY_DSA_SIGN:
			case SSL_PKEY_DH_DSA:
				rsign = TLSEXT_signature_dsa;
				default_nid = NID_dsaWithSHA1;
				break;

			case SSL_PKEY_ECC:
				rsign = TLSEXT_signature_ecdsa;
				default_nid = NID_ecdsa_with_SHA1;
				break;

			default:
				default_nid = -1;
				break;
				}
			}
		/* If peer sent no signature algorithms extension and we
		 * have set preferred signature algorithms check we support
		 * sha1.
		 */
		if (default_nid > 0 && c->conf_sigalgs)
			{
			size_t j;
			const unsigned char *p = c->conf_sigalgs;
			for (j = 0; j < c->conf_sigalgslen; j += 2, p += 2)
				{
				if (p[0] == TLSEXT_hash_sha1 && p[1] == rsign)
					break;
				}
			if (j == c->conf_sigalgslen)
				{
				if (check_flags)
					goto skip_sigs;
				else
					goto end;
				}
			}
		/* Check signature algorithm of each cert in chain */
		if (!tls1_check_sig_alg(c, x, default_nid))
			{
			if (!check_flags) goto end;
			}
		else
			rv |= CERT_PKEY_EE_SIGNATURE;
		rv |= CERT_PKEY_CA_SIGNATURE;
		for (i = 0; i < sk_X509_num(chain); i++)
			{
			if (!tls1_check_sig_alg(c, sk_X509_value(chain, i),
							default_nid))
				{
				if (check_flags)
					{
					rv &= ~CERT_PKEY_CA_SIGNATURE;
					break;
					}
				else
					goto end;
				}
			}
		}
	/* Else not TLS 1.2, so mark EE and CA signing algorithms OK */
	else if(check_flags)
		rv |= CERT_PKEY_EE_SIGNATURE|CERT_PKEY_CA_SIGNATURE;
	skip_sigs:
	/* Check cert parameters are consistent */
	if (tls1_check_cert_param(s, x, check_flags ? 1 : 2))
		rv |= CERT_PKEY_EE_PARAM;
	else if (!check_flags)
		goto end;
	if (!s->server)
		rv |= CERT_PKEY_CA_PARAM;
	/* In strict mode check rest of chain too */
	else if (strict_mode)
		{
		rv |= CERT_PKEY_CA_PARAM;
		for (i = 0; i < sk_X509_num(chain); i++)
			{
			X509 *ca = sk_X509_value(chain, i);
			if (!tls1_check_cert_param(s, ca, 0))
				{
				if (check_flags)
					{
					rv &= ~CERT_PKEY_CA_PARAM;
					break;
					}
				else
					goto end;
				}
			}
		}
	if (!s->server && strict_mode)
		{
		STACK_OF(X509_NAME) *ca_dn;
		int check_type = 0;
		switch (pk->type)
			{
		case EVP_PKEY_RSA:
			check_type = TLS_CT_RSA_SIGN;
			break;
		case EVP_PKEY_DSA:
			check_type = TLS_CT_DSS_SIGN;
			break;
		case EVP_PKEY_EC:
			check_type = TLS_CT_ECDSA_SIGN;
			break;
		case EVP_PKEY_DH:
		case EVP_PKEY_DHX:
				{
				int cert_type = X509_certificate_type(x, pk);
				if (cert_type & EVP_PKS_RSA)
					check_type = TLS_CT_RSA_FIXED_DH;
				if (cert_type & EVP_PKS_DSA)
					check_type = TLS_CT_DSS_FIXED_DH;
				}
			}
		if (check_type)
			{
			const unsigned char *ctypes;
			int ctypelen;
			if (c->ctypes)
				{
				ctypes = c->ctypes;
				ctypelen = (int)c->ctype_num;
				}
			else
				{
				ctypes = (unsigned char *)s->s3->tmp.ctype;
				ctypelen = s->s3->tmp.ctype_num;
				}
			for (i = 0; i < ctypelen; i++)
				{
				if (ctypes[i] == check_type)
					{
					rv |= CERT_PKEY_CERT_TYPE;
					break;
					}
				}
			if (!(rv & CERT_PKEY_CERT_TYPE) && !check_flags)
				goto end;
			}
		else
			rv |= CERT_PKEY_CERT_TYPE;


		ca_dn = s->s3->tmp.ca_names;

		if (!sk_X509_NAME_num(ca_dn))
			rv |= CERT_PKEY_ISSUER_NAME;

		if (!(rv & CERT_PKEY_ISSUER_NAME))
			{
			if (ssl_check_ca_name(ca_dn, x))
				rv |= CERT_PKEY_ISSUER_NAME;
			}
		if (!(rv & CERT_PKEY_ISSUER_NAME))
			{
			for (i = 0; i < sk_X509_num(chain); i++)
				{
				X509 *xtmp = sk_X509_value(chain, i);
				if (ssl_check_ca_name(ca_dn, xtmp))
					{
					rv |= CERT_PKEY_ISSUER_NAME;
					break;
					}
				}
			}
		if (!check_flags && !(rv & CERT_PKEY_ISSUER_NAME))
			goto end;
		}
	else
		rv |= CERT_PKEY_ISSUER_NAME|CERT_PKEY_CERT_TYPE;

	if (!check_flags || (rv & check_flags) == check_flags)
		rv |= CERT_PKEY_VALID;

	end:

	if (TLS1_get_version(s) >= TLS1_2_VERSION)
		{
		if (cpk->valid_flags & CERT_PKEY_EXPLICIT_SIGN)
			rv |= CERT_PKEY_EXPLICIT_SIGN|CERT_PKEY_SIGN;
		else if (cpk->digest)
			rv |= CERT_PKEY_SIGN;
		}
	else
		rv |= CERT_PKEY_SIGN|CERT_PKEY_EXPLICIT_SIGN;

	/* When checking a CERT_PKEY structure all flags are irrelevant
	 * if the chain is invalid.
	 */
	if (!check_flags)
		{
		if (rv & CERT_PKEY_VALID)
			cpk->valid_flags = rv;
		else
			{
			/* Preserve explicit sign flag, clear rest */
			cpk->valid_flags &= CERT_PKEY_EXPLICIT_SIGN;
			return 0;
			}
		}
	return rv;
	}
