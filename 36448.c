int compat_udpv6_getsockopt(struct sock *sk, int level, int optname,
			    char __user *optval, int __user *optlen)
{
	if (level == SOL_UDP  ||  level == SOL_UDPLITE)
		return udp_lib_getsockopt(sk, level, optname, optval, optlen);
	return compat_ipv6_getsockopt(sk, level, optname, optval, optlen);
}
