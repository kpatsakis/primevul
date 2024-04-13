send_query_to_peer(peer_t *p)
{
	/* Why do we need to bind()?
	 * See what happens when we don't bind:
	 *
	 * socket(PF_INET, SOCK_DGRAM, IPPROTO_IP) = 3
	 * setsockopt(3, SOL_IP, IP_TOS, [16], 4) = 0
	 * gettimeofday({1259071266, 327885}, NULL) = 0
	 * sendto(3, "xxx", 48, MSG_DONTWAIT, {sa_family=AF_INET, sin_port=htons(123), sin_addr=inet_addr("10.34.32.125")}, 16) = 48
	 * ^^^ we sent it from some source port picked by kernel.
	 * time(NULL)              = 1259071266
	 * write(2, "ntpd: entering poll 15 secs\n", 28) = 28
	 * poll([{fd=3, events=POLLIN}], 1, 15000) = 1 ([{fd=3, revents=POLLIN}])
	 * recv(3, "yyy", 68, MSG_DONTWAIT) = 48
	 * ^^^ this recv will receive packets to any local port!
	 *
	 * Uncomment this and use strace to see it in action:
	 */
#define PROBE_LOCAL_ADDR /* { len_and_sockaddr lsa; lsa.len = LSA_SIZEOF_SA; getsockname(p->query.fd, &lsa.u.sa, &lsa.len); } */

	if (p->p_fd == -1) {
		int fd, family;
		len_and_sockaddr *local_lsa;

		family = p->p_lsa->u.sa.sa_family;
		p->p_fd = fd = xsocket_type(&local_lsa, family, SOCK_DGRAM);
		/* local_lsa has "null" address and port 0 now.
		 * bind() ensures we have a *particular port* selected by kernel
		 * and remembered in p->p_fd, thus later recv(p->p_fd)
		 * receives only packets sent to this port.
		 */
		PROBE_LOCAL_ADDR
		xbind(fd, &local_lsa->u.sa, local_lsa->len);
		PROBE_LOCAL_ADDR
#if ENABLE_FEATURE_IPV6
		if (family == AF_INET)
#endif
			setsockopt_int(fd, IPPROTO_IP, IP_TOS, IPTOS_LOWDELAY);
		free(local_lsa);
	}

	/* Emit message _before_ attempted send. Think of a very short
	 * roundtrip networks: we need to go back to recv loop ASAP,
	 * to reduce delay. Printing messages after send works against that.
	 */
	VERB1 bb_error_msg("sending query to %s", p->p_dotted);

	/*
	 * Send out a random 64-bit number as our transmit time.  The NTP
	 * server will copy said number into the originate field on the
	 * response that it sends us.  This is totally legal per the SNTP spec.
	 *
	 * The impact of this is two fold: we no longer send out the current
	 * system time for the world to see (which may aid an attacker), and
	 * it gives us a (not very secure) way of knowing that we're not
	 * getting spoofed by an attacker that can't capture our traffic
	 * but can spoof packets from the NTP server we're communicating with.
	 *
	 * Save the real transmit timestamp locally.
	 */
	p->p_xmt_msg.m_xmttime.int_partl = rand();
	p->p_xmt_msg.m_xmttime.fractionl = rand();
	p->p_xmttime = gettime1900d();

	/* Were doing it only if sendto worked, but
	 * loss of sync detection needs reachable_bits updated
	 * even if sending fails *locally*:
	 * "network is unreachable" because cable was pulled?
	 * We still need to declare "unsync" if this condition persists.
	 */
	p->reachable_bits <<= 1;

	if (do_sendto(p->p_fd, /*from:*/ NULL, /*to:*/ &p->p_lsa->u.sa, /*addrlen:*/ p->p_lsa->len,
			&p->p_xmt_msg, NTP_MSGSIZE_NOAUTH) == -1
	) {
		close(p->p_fd);
		p->p_fd = -1;
		/*
		 * We know that we sent nothing.
		 * We can retry *soon* without fearing
		 * that we are flooding the peer.
		 */
		set_next(p, RETRY_INTERVAL);
		return;
	}

	set_next(p, RESPONSE_INTERVAL);
}
