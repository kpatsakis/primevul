const EVP_MD *tls12_get_hash(unsigned char hash_alg)
	{
	const tls12_hash_info *inf;
#ifndef OPENSSL_FIPS
	if (hash_alg == TLSEXT_hash_md5 && FIPS_mode())
		return NULL;
#endif
	inf = tls12_get_hash_info(hash_alg);
	if (!inf || !inf->mfunc)
		return NULL; 
	return inf->mfunc();
	}
