static int ssl_security_cert_sig(SSL *s, SSL_CTX *ctx, X509 *x, int op)
	{
	/* Lookup signature algorithm digest */
	int secbits = -1, md_nid = NID_undef, sig_nid;
	sig_nid = X509_get_signature_nid(x);
	if (sig_nid && OBJ_find_sigid_algs(sig_nid, &md_nid, NULL))
		{
		const EVP_MD *md;
		if (md_nid && (md = EVP_get_digestbynid(md_nid)))
				secbits = EVP_MD_size(md) * 4;
		}
	if (s)
		return ssl_security(s, op, secbits, md_nid, x);
	else
		return ssl_ctx_security(ctx, op, secbits, md_nid, x);
	}
