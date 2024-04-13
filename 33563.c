int ssl_prepare_serverhello_tlsext(SSL *s)
	{
#ifndef OPENSSL_NO_EC
	/* If we are server and using an ECC cipher suite, send the point formats we support 
	 * if the client sent us an ECPointsFormat extension.  Note that the server is not
	 * supposed to send an EllipticCurves extension.
	 */

	unsigned long alg_k = s->s3->tmp.new_cipher->algorithm_mkey;
	unsigned long alg_a = s->s3->tmp.new_cipher->algorithm_auth;
	int using_ecc = (alg_k & (SSL_kEECDH|SSL_kECDHr|SSL_kECDHe)) || (alg_a & SSL_aECDSA);
	using_ecc = using_ecc && (s->session->tlsext_ecpointformatlist != NULL);
	
	if (using_ecc)
		{
		if (s->tlsext_ecpointformatlist != NULL) OPENSSL_free(s->tlsext_ecpointformatlist);
		if ((s->tlsext_ecpointformatlist = OPENSSL_malloc(3)) == NULL)
			{
			SSLerr(SSL_F_SSL_PREPARE_SERVERHELLO_TLSEXT,ERR_R_MALLOC_FAILURE);
			return -1;
			}
		s->tlsext_ecpointformatlist_length = 3;
		s->tlsext_ecpointformatlist[0] = TLSEXT_ECPOINTFORMAT_uncompressed;
		s->tlsext_ecpointformatlist[1] = TLSEXT_ECPOINTFORMAT_ansiX962_compressed_prime;
		s->tlsext_ecpointformatlist[2] = TLSEXT_ECPOINTFORMAT_ansiX962_compressed_char2;
		}
#endif /* OPENSSL_NO_EC */

	return 1;
	}
