rtadv_make_socket (vrf_id_t vrf_id)
{
  int sock;
  int ret;
  struct icmp6_filter filter;

  if ( zserv_privs.change (ZPRIVS_RAISE) )
       zlog_err ("rtadv_make_socket: could not raise privs, %s",
                  safe_strerror (errno) );
                  
  sock = vrf_socket (AF_INET6, SOCK_RAW, IPPROTO_ICMPV6, vrf_id);

  if ( zserv_privs.change (ZPRIVS_LOWER) )
       zlog_err ("rtadv_make_socket: could not lower privs, %s",
       			 safe_strerror (errno) );

  /* When we can't make ICMPV6 socket simply back.  Router
     advertisement feature will not be supported. */
  if (sock < 0)
    {
      close (sock);
      return -1;
    }

  ret = setsockopt_ipv6_pktinfo (sock, 1);
  if (ret < 0)
    {
      close (sock);
      return ret;
    }
  ret = setsockopt_ipv6_multicast_loop (sock, 0);
  if (ret < 0)
    {
      close (sock);
      return ret;
    }
  ret = setsockopt_ipv6_unicast_hops (sock, 255);
  if (ret < 0)
    {
      close (sock);
      return ret;
    }
  ret = setsockopt_ipv6_multicast_hops (sock, 255);
  if (ret < 0)
    {
      close (sock);
      return ret;
    }
  ret = setsockopt_ipv6_hoplimit (sock, 1);
  if (ret < 0)
    {
      close (sock);
      return ret;
    }

  ICMP6_FILTER_SETBLOCKALL(&filter);
  ICMP6_FILTER_SETPASS (ND_ROUTER_SOLICIT, &filter);
  ICMP6_FILTER_SETPASS (ND_ROUTER_ADVERT, &filter);

  ret = setsockopt (sock, IPPROTO_ICMPV6, ICMP6_FILTER, &filter,
		    sizeof (struct icmp6_filter));
  if (ret < 0)
    {
      zlog_info ("ICMP6_FILTER set fail: %s", safe_strerror (errno));
      return ret;
    }

  return sock;
}
