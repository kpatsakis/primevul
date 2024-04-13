static void tls1_get_curvelist(SSL *s, int sess,
					const unsigned char **pcurves,
					size_t *pcurveslen)
	{
	if (sess)
		{
		*pcurves = s->session->tlsext_ellipticcurvelist;
		*pcurveslen = s->session->tlsext_ellipticcurvelist_length;
		return;
		}
	/* For Suite B mode only include P-256, P-384 */
	switch (tls1_suiteb(s))
		{
	case SSL_CERT_FLAG_SUITEB_128_LOS:
		*pcurves = suiteb_curves;
		*pcurveslen = sizeof(suiteb_curves);
		break;

	case SSL_CERT_FLAG_SUITEB_128_LOS_ONLY:
		*pcurves = suiteb_curves;
		*pcurveslen = 2;
		break;

	case SSL_CERT_FLAG_SUITEB_192_LOS:
		*pcurves = suiteb_curves + 2;
		*pcurveslen = 2;
		break;
	default:
		*pcurves = s->tlsext_ellipticcurvelist;
		*pcurveslen = s->tlsext_ellipticcurvelist_length;
		}
	if (!*pcurves)
		{
		*pcurves = eccurves_default;
		*pcurveslen = sizeof(eccurves_default);
		}
	}
