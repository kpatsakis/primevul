eXosip_find_free_port (struct eXosip_t *excontext, int free_port, int transport)
{
  int res1;
  int res2;
  struct addrinfo *addrinfo_rtp = NULL;
  struct addrinfo *curinfo_rtp;
  struct addrinfo *addrinfo_rtcp = NULL;
  struct addrinfo *curinfo_rtcp;
  int sock;
  int count;

  for (count = 0; count < 8; count++) {
    if (excontext->ipv6_enable == 0)
      res1 = _eXosip_get_addrinfo (excontext, &addrinfo_rtp, "0.0.0.0", free_port + count * 2, transport);
    else
      res1 = _eXosip_get_addrinfo (excontext, &addrinfo_rtp, "::", free_port + count * 2, transport);
    if (res1 != 0)
      return res1;
    if (excontext->ipv6_enable == 0)
      res2 = _eXosip_get_addrinfo (excontext, &addrinfo_rtcp, "0.0.0.0", free_port + count * 2 + 1, transport);
    else
      res2 = _eXosip_get_addrinfo (excontext, &addrinfo_rtcp, "::", free_port + count * 2 + 1, transport);
    if (res2 != 0) {
      _eXosip_freeaddrinfo (addrinfo_rtp);
      return res2;
    }

    sock = -1;
    for (curinfo_rtp = addrinfo_rtp; curinfo_rtp; curinfo_rtp = curinfo_rtp->ai_next) {
      if (curinfo_rtp->ai_protocol && curinfo_rtp->ai_protocol != transport) {
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_INFO3, NULL, "eXosip: Skipping protocol %d\n", curinfo_rtp->ai_protocol));
        continue;
      }

      sock = (int) socket (curinfo_rtp->ai_family, curinfo_rtp->ai_socktype, curinfo_rtp->ai_protocol);
      if (sock < 0) {
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot create socket!\n"));
        continue;
      }

      if (curinfo_rtp->ai_family == AF_INET6) {
#ifdef IPV6_V6ONLY
        if (setsockopt_ipv6only (sock)) {
          close (sock);
          sock = -1;
          OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot set socket option!\n"));
          continue;
        }
#endif /* IPV6_V6ONLY */
      }

      res1 = bind (sock, curinfo_rtp->ai_addr, curinfo_rtp->ai_addrlen);
      if (res1 < 0) {
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_WARNING, NULL, "eXosip: Cannot bind socket node: 0.0.0.0 family:%d\n", curinfo_rtp->ai_family));
        close (sock);
        sock = -1;
        continue;
      }
      break;
    }

    _eXosip_freeaddrinfo (addrinfo_rtp);

    if (sock == -1) {
      _eXosip_freeaddrinfo (addrinfo_rtcp);
      continue;
    }

    close (sock);
    sock = -1;
    for (curinfo_rtcp = addrinfo_rtcp; curinfo_rtcp; curinfo_rtcp = curinfo_rtcp->ai_next) {
      if (curinfo_rtcp->ai_protocol && curinfo_rtcp->ai_protocol != transport) {
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_INFO3, NULL, "eXosip: Skipping protocol %d\n", curinfo_rtcp->ai_protocol));
        continue;
      }

      sock = (int) socket (curinfo_rtcp->ai_family, curinfo_rtcp->ai_socktype, curinfo_rtcp->ai_protocol);
      if (sock < 0) {
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot create socket!\n"));
        continue;
      }

      if (curinfo_rtcp->ai_family == AF_INET6) {
#ifdef IPV6_V6ONLY
        if (setsockopt_ipv6only (sock)) {
          close (sock);
          sock = -1;
          OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot set socket option!\n"));
          continue;
        }
#endif /* IPV6_V6ONLY */
      }

      res1 = bind (sock, curinfo_rtcp->ai_addr, curinfo_rtcp->ai_addrlen);
      if (res1 < 0) {
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_WARNING, NULL, "eXosip: Cannot bind socket node: 0.0.0.0 family:%d\n", curinfo_rtp->ai_family));
        close (sock);
        sock = -1;
        continue;
      }
      break;
    }

    _eXosip_freeaddrinfo (addrinfo_rtcp);

    /* the pair must be free */
    if (sock == -1)
      continue;

    close (sock);
    sock = -1;
    return free_port + count * 2;
  }

  /* just get a free port */
  if (excontext->ipv6_enable == 0)
    res1 = _eXosip_get_addrinfo (excontext, &addrinfo_rtp, "0.0.0.0", 0, transport);
  else
    res1 = _eXosip_get_addrinfo (excontext, &addrinfo_rtp, "::", 0, transport);

  if (res1)
    return res1;

  sock = -1;
  for (curinfo_rtp = addrinfo_rtp; curinfo_rtp; curinfo_rtp = curinfo_rtp->ai_next) {
    socklen_t len;
    struct sockaddr_storage ai_addr;

    if (curinfo_rtp->ai_protocol && curinfo_rtp->ai_protocol != transport) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_INFO3, NULL, "eXosip: Skipping protocol %d\n", curinfo_rtp->ai_protocol));
      continue;
    }

    sock = (int) socket (curinfo_rtp->ai_family, curinfo_rtp->ai_socktype, curinfo_rtp->ai_protocol);
    if (sock < 0) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot create socket!\n"));
      continue;
    }

    if (curinfo_rtp->ai_family == AF_INET6) {
#ifdef IPV6_V6ONLY
      if (setsockopt_ipv6only (sock)) {
        close (sock);
        sock = -1;
        OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot set socket option!\n"));
        continue;
      }
#endif /* IPV6_V6ONLY */
    }

    res1 = bind (sock, curinfo_rtp->ai_addr, curinfo_rtp->ai_addrlen);
    if (res1 < 0) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_WARNING, NULL, "eXosip: Cannot bind socket node: 0.0.0.0 family:%d\n", curinfo_rtp->ai_family));
      close (sock);
      sock = -1;
      continue;
    }

    len = sizeof (ai_addr);
    res1 = getsockname (sock, (struct sockaddr *) &ai_addr, &len);
    if (res1 != 0) {
      close (sock);
      sock = -1;
      continue;
    }

    close (sock);
    sock = -1;
    _eXosip_freeaddrinfo (addrinfo_rtp);

    if (curinfo_rtp->ai_family == AF_INET)
      return ntohs (((struct sockaddr_in *) &ai_addr)->sin_port);
    else
      return ntohs (((struct sockaddr_in6 *) &ai_addr)->sin6_port);
  }

  _eXosip_freeaddrinfo (addrinfo_rtp);

  if (sock != -1) {
    close (sock);
    sock = -1;
  }

  return OSIP_UNDEFINED_ERROR;
}
