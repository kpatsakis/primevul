DH *ssl_get_auto_dh(SSL *s)
	{
	int dh_secbits = 80;
	if (s->cert->dh_tmp_auto == 2)
		return DH_get_1024_160();
	if (s->s3->tmp.new_cipher->algorithm_auth & SSL_aNULL)
		{
		if (s->s3->tmp.new_cipher->strength_bits == 256)
			dh_secbits = 128;
		else
			dh_secbits = 80;
		}
	else
		{
		CERT_PKEY *cpk = ssl_get_server_send_pkey(s);
		dh_secbits = EVP_PKEY_security_bits(cpk->privatekey);
		}

	if (dh_secbits >= 128)
		{
		DH *dhp = DH_new();
		if (!dhp)
			return NULL;
		dhp->g = BN_new();
		if (dhp->g)
			BN_set_word(dhp->g, 2);
		if (dh_secbits >= 192)
			dhp->p = get_rfc3526_prime_8192(NULL);
		else
			dhp->p = get_rfc3526_prime_3072(NULL);
		if (!dhp->p || !dhp->g)
			{
			DH_free(dhp);
			return NULL;
			}
		return dhp;
		}
	if (dh_secbits >= 112)
		return DH_get_2048_224();
	return DH_get_1024_160();
	}
