void inet_set_tos(int fd, const struct sockaddr_storage *from, int tos)
{
#ifdef IP_TOS
	if (from->ss_family == AF_INET)
		setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
#endif
#ifdef IPV6_TCLASS
	if (from->ss_family == AF_INET6) {
		if (IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)from)->sin6_addr))
			/* v4-mapped addresses need IP_TOS */
			setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
		else
			setsockopt(fd, IPPROTO_IPV6, IPV6_TCLASS, &tos, sizeof(tos));
	}
#endif
}
