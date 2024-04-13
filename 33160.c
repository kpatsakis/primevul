static void tls1_get_formatlist(SSL *s, const unsigned char **pformats,
					size_t *pformatslen)
	{
	/* If we have a custom point format list use it otherwise
	 * use default */
	if (s->tlsext_ecpointformatlist)
		{
		*pformats = s->tlsext_ecpointformatlist;
		*pformatslen = s->tlsext_ecpointformatlist_length;
		}
	else
		{
		*pformats = ecformats_default;
		/* For Suite B we don't support char2 fields */
		if (tls1_suiteb(s))
			*pformatslen = sizeof(ecformats_default) - 1;
		else
			*pformatslen = sizeof(ecformats_default);
		}
	}
