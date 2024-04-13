asmlinkage long sys_oabi_sendto(int fd, void __user *buff,
				size_t len, unsigned flags,
				struct sockaddr __user *addr,
				int addrlen)
{
	sa_family_t sa_family;
	if (addrlen == 112 &&
	    get_user(sa_family, &addr->sa_family) == 0 &&
	    sa_family == AF_UNIX)
			addrlen = 110;
	return sys_sendto(fd, buff, len, flags, addr, addrlen);
}
