static int sig_cb(const char *elem, int len, void *arg)
	{
	sig_cb_st *sarg = arg;
	size_t i;
	char etmp[20], *p;
	int sig_alg, hash_alg;
	if (sarg->sigalgcnt == MAX_SIGALGLEN)
		return 0;
	if (len > (int)(sizeof(etmp) - 1))
		return 0;
	memcpy(etmp, elem, len);
	etmp[len] = 0;
	p = strchr(etmp, '+');
	if (!p)
		return 0;
	*p = 0;
	p++;
	if (!*p)
		return 0;

	if (!strcmp(etmp, "RSA"))
		sig_alg = EVP_PKEY_RSA;
	else if (!strcmp(etmp, "DSA"))
		sig_alg = EVP_PKEY_DSA;
	else if (!strcmp(etmp, "ECDSA"))
		sig_alg = EVP_PKEY_EC;
	else return 0;

	hash_alg = OBJ_sn2nid(p);
	if (hash_alg == NID_undef)
		hash_alg = OBJ_ln2nid(p);
	if (hash_alg == NID_undef)
		return 0;

	for (i = 0; i < sarg->sigalgcnt; i+=2)
		{
		if (sarg->sigalgs[i] == sig_alg
			&& sarg->sigalgs[i + 1] == hash_alg)
			return 0;
		}
	sarg->sigalgs[sarg->sigalgcnt++] = hash_alg;
	sarg->sigalgs[sarg->sigalgcnt++] = sig_alg;
	return 1;
	}
