static int tls12_sigalg_allowed(SSL *s, int op, const unsigned char *ptmp)
	{
	/* See if we have an entry in the hash table and it is enabled */
	const tls12_hash_info *hinf = tls12_get_hash_info(ptmp[0]);
	if (!hinf || !hinf->mfunc)
		return 0;
	/* See if public key algorithm allowed */
	if (tls12_get_pkey_idx(ptmp[1]) == -1)
		return 0;
	/* Finally see if security callback allows it */
	return ssl_security(s, op, hinf->secbits, hinf->nid, (void *)ptmp);
	}
