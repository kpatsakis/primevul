rtadv_send_packet (int sock, struct interface *ifp)
{
  struct msghdr msg;
  struct iovec iov;
  struct cmsghdr  *cmsgptr;
  struct in6_pktinfo *pkt;
  struct sockaddr_in6 addr;
#ifdef HAVE_STRUCT_SOCKADDR_DL
  struct sockaddr_dl *sdl;
#endif /* HAVE_STRUCT_SOCKADDR_DL */
  static void *adata = NULL;
  unsigned char buf[RTADV_MSG_SIZE];
  struct nd_router_advert *rtadv;
  int ret;
  int len = 0;
  struct zebra_if *zif;
  struct rtadv_prefix *rprefix;
  u_char all_nodes_addr[] = {0xff,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
  struct listnode *node;
  u_int16_t pkt_RouterLifetime;

  /*
   * Allocate control message bufffer.  This is dynamic because
   * CMSG_SPACE is not guaranteed not to call a function.  Note that
   * the size will be different on different architectures due to
   * differing alignment rules.
   */
  if (adata == NULL)
    {
      /* XXX Free on shutdown. */
      adata = malloc(CMSG_SPACE(sizeof(struct in6_pktinfo)));
	   
      if (adata == NULL)
	zlog_err("rtadv_send_packet: can't malloc control data\n");
    }

  /* Logging of packet. */
  if (IS_ZEBRA_DEBUG_PACKET)
    zlog_debug ("Router advertisement send to %s", ifp->name);

  /* Fill in sockaddr_in6. */
  memset (&addr, 0, sizeof (struct sockaddr_in6));
  addr.sin6_family = AF_INET6;
#ifdef SIN6_LEN
  addr.sin6_len = sizeof (struct sockaddr_in6);
#endif /* SIN6_LEN */
  addr.sin6_port = htons (IPPROTO_ICMPV6);
  IPV6_ADDR_COPY (&addr.sin6_addr, all_nodes_addr);

  /* Fetch interface information. */
  zif = ifp->info;

  /* Make router advertisement message. */
  rtadv = (struct nd_router_advert *) buf;

  rtadv->nd_ra_type = ND_ROUTER_ADVERT;
  rtadv->nd_ra_code = 0;
  rtadv->nd_ra_cksum = 0;

  rtadv->nd_ra_curhoplimit = 64;

  /* RFC4191: Default Router Preference is 0 if Router Lifetime is 0. */
  rtadv->nd_ra_flags_reserved =
    zif->rtadv.AdvDefaultLifetime == 0 ? 0 : zif->rtadv.DefaultPreference;
  rtadv->nd_ra_flags_reserved <<= 3;

  if (zif->rtadv.AdvManagedFlag)
    rtadv->nd_ra_flags_reserved |= ND_RA_FLAG_MANAGED;
  if (zif->rtadv.AdvOtherConfigFlag)
    rtadv->nd_ra_flags_reserved |= ND_RA_FLAG_OTHER;
  if (zif->rtadv.AdvHomeAgentFlag)
    rtadv->nd_ra_flags_reserved |= ND_RA_FLAG_HOME_AGENT;
  /* Note that according to Neighbor Discovery (RFC 4861 [18]),
   * AdvDefaultLifetime is by default based on the value of
   * MaxRtrAdvInterval.  AdvDefaultLifetime is used in the Router Lifetime
   * field of Router Advertisements.  Given that this field is expressed
   * in seconds, a small MaxRtrAdvInterval value can result in a zero
   * value for this field.  To prevent this, routers SHOULD keep
   * AdvDefaultLifetime in at least one second, even if the use of
   * MaxRtrAdvInterval would result in a smaller value. -- RFC6275, 7.5 */
  pkt_RouterLifetime = zif->rtadv.AdvDefaultLifetime != -1 ?
    zif->rtadv.AdvDefaultLifetime :
    MAX (1, 0.003 * zif->rtadv.MaxRtrAdvInterval);
  rtadv->nd_ra_router_lifetime = htons (pkt_RouterLifetime);
  rtadv->nd_ra_reachable = htonl (zif->rtadv.AdvReachableTime);
  rtadv->nd_ra_retransmit = htonl (0);

  len = sizeof (struct nd_router_advert);

  /* If both the Home Agent Preference and Home Agent Lifetime are set to
   * their default values specified above, this option SHOULD NOT be
   * included in the Router Advertisement messages sent by this home
   * agent. -- RFC6275, 7.4 */
  if
  (
    zif->rtadv.AdvHomeAgentFlag &&
    (zif->rtadv.HomeAgentPreference || zif->rtadv.HomeAgentLifetime != -1)
  )
    {
      struct nd_opt_homeagent_info *ndopt_hai = 
	(struct nd_opt_homeagent_info *)(buf + len);
      ndopt_hai->nd_opt_hai_type = ND_OPT_HA_INFORMATION;
      ndopt_hai->nd_opt_hai_len = 1;
      ndopt_hai->nd_opt_hai_reserved = 0;
      ndopt_hai->nd_opt_hai_preference = htons(zif->rtadv.HomeAgentPreference);
      /* 16-bit unsigned integer.  The lifetime associated with the home
       * agent in units of seconds.  The default value is the same as the
       * Router Lifetime, as specified in the main body of the Router
       * Advertisement.  The maximum value corresponds to 18.2 hours.  A
       * value of 0 MUST NOT be used. -- RFC6275, 7.5 */
      ndopt_hai->nd_opt_hai_lifetime = htons
      (
        zif->rtadv.HomeAgentLifetime != -1 ?
        zif->rtadv.HomeAgentLifetime :
        MAX (1, pkt_RouterLifetime) /* 0 is OK for RL, but not for HAL*/
      );
      len += sizeof(struct nd_opt_homeagent_info);
    }

  if (zif->rtadv.AdvIntervalOption)
    {
      struct nd_opt_adv_interval *ndopt_adv = 
	(struct nd_opt_adv_interval *)(buf + len);
      ndopt_adv->nd_opt_ai_type = ND_OPT_ADV_INTERVAL;
      ndopt_adv->nd_opt_ai_len = 1;
      ndopt_adv->nd_opt_ai_reserved = 0;
      ndopt_adv->nd_opt_ai_interval = htonl(zif->rtadv.MaxRtrAdvInterval);
      len += sizeof(struct nd_opt_adv_interval);
    }

  /* Fill in prefix. */
  for (ALL_LIST_ELEMENTS_RO (zif->rtadv.AdvPrefixList, node, rprefix))
    {
      struct nd_opt_prefix_info *pinfo;

      pinfo = (struct nd_opt_prefix_info *) (buf + len);

      pinfo->nd_opt_pi_type = ND_OPT_PREFIX_INFORMATION;
      pinfo->nd_opt_pi_len = 4;
      pinfo->nd_opt_pi_prefix_len = rprefix->prefix.prefixlen;

      pinfo->nd_opt_pi_flags_reserved = 0;
      if (rprefix->AdvOnLinkFlag)
	pinfo->nd_opt_pi_flags_reserved |= ND_OPT_PI_FLAG_ONLINK;
      if (rprefix->AdvAutonomousFlag)
	pinfo->nd_opt_pi_flags_reserved |= ND_OPT_PI_FLAG_AUTO;
      if (rprefix->AdvRouterAddressFlag)
	pinfo->nd_opt_pi_flags_reserved |= ND_OPT_PI_FLAG_RADDR;

      pinfo->nd_opt_pi_valid_time = htonl (rprefix->AdvValidLifetime);
      pinfo->nd_opt_pi_preferred_time = htonl (rprefix->AdvPreferredLifetime);
      pinfo->nd_opt_pi_reserved2 = 0;

      IPV6_ADDR_COPY (&pinfo->nd_opt_pi_prefix, &rprefix->prefix.prefix);

#ifdef DEBUG
      {
	u_char buf[INET6_ADDRSTRLEN];

	zlog_debug ("DEBUG %s", inet_ntop (AF_INET6, &pinfo->nd_opt_pi_prefix, 
	           buf, INET6_ADDRSTRLEN));

      }
#endif /* DEBUG */

      len += sizeof (struct nd_opt_prefix_info);
    }

  /* Hardware address. */
  if (ifp->hw_addr_len != 0)
    {
      buf[len++] = ND_OPT_SOURCE_LINKADDR;

      /* Option length should be rounded up to next octet if
         the link address does not end on an octet boundary. */
      buf[len++] = (ifp->hw_addr_len + 9) >> 3;

      memcpy (buf + len, ifp->hw_addr, ifp->hw_addr_len);
      len += ifp->hw_addr_len;

      /* Pad option to end on an octet boundary. */
      memset (buf + len, 0, -(ifp->hw_addr_len + 2) & 0x7);
      len += -(ifp->hw_addr_len + 2) & 0x7;
    }

  /* MTU */
  if (zif->rtadv.AdvLinkMTU)
    {
      struct nd_opt_mtu * opt = (struct nd_opt_mtu *) (buf + len);
      opt->nd_opt_mtu_type = ND_OPT_MTU;
      opt->nd_opt_mtu_len = 1;
      opt->nd_opt_mtu_reserved = 0;
      opt->nd_opt_mtu_mtu = htonl (zif->rtadv.AdvLinkMTU);
      len += sizeof (struct nd_opt_mtu);
    }

  msg.msg_name = (void *) &addr;
  msg.msg_namelen = sizeof (struct sockaddr_in6);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = (void *) adata;
  msg.msg_controllen = CMSG_SPACE(sizeof(struct in6_pktinfo));
  msg.msg_flags = 0;
  iov.iov_base = buf;
  iov.iov_len = len;

  cmsgptr = ZCMSG_FIRSTHDR(&msg);
  cmsgptr->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo));
  cmsgptr->cmsg_level = IPPROTO_IPV6;
  cmsgptr->cmsg_type = IPV6_PKTINFO;

  pkt = (struct in6_pktinfo *) CMSG_DATA (cmsgptr);
  memset (&pkt->ipi6_addr, 0, sizeof (struct in6_addr));
  pkt->ipi6_ifindex = ifp->ifindex;

  ret = sendmsg (sock, &msg, 0);
  if (ret < 0)
    {
      zlog_err ("rtadv_send_packet: sendmsg %d (%s)\n",
		errno, safe_strerror(errno));
    }
}
