set_font_authorizations(char **authorizations, int *authlen, pointer client)
{
#define AUTHORIZATION_NAME "hp-hostname-1"
#if defined(TCPCONN) || defined(STREAMSCONN)
    static char *result = NULL;
    static char *p = NULL;

    if (p == NULL)
    {
	char hname[1024], *hnameptr;
	unsigned int len;
#if defined(IPv6) && defined(AF_INET6)
	struct addrinfo hints, *ai = NULL;
#else
	struct hostent *host;
#ifdef XTHREADS_NEEDS_BYNAMEPARAMS
	_Xgethostbynameparams hparams;
#endif
#endif

	gethostname(hname, 1024);
#if defined(IPv6) && defined(AF_INET6)
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(hname, NULL, &hints, &ai) == 0) {
	    hnameptr = ai->ai_canonname;
	} else {
	    hnameptr = hname;
	}
#else
	host = _XGethostbyname(hname, hparams);
	if (host == NULL)
	    hnameptr = hname;
	else
	    hnameptr = host->h_name;
#endif

	len = strlen(hnameptr) + 1;
	result = malloc(len + sizeof(AUTHORIZATION_NAME) + 4);

	p = result;
        *p++ = sizeof(AUTHORIZATION_NAME) >> 8;
        *p++ = sizeof(AUTHORIZATION_NAME) & 0xff;
        *p++ = (len) >> 8;
        *p++ = (len & 0xff);

	memmove(p, AUTHORIZATION_NAME, sizeof(AUTHORIZATION_NAME));
	p += sizeof(AUTHORIZATION_NAME);
	memmove(p, hnameptr, len);
	p += len;
#if defined(IPv6) && defined(AF_INET6)
	if (ai) {
	    freeaddrinfo(ai);
	}
#endif
    }
    *authlen = p - result;
    *authorizations = result;
    return 1;
#else /* TCPCONN */
    return 0;
#endif /* TCPCONN */
}
