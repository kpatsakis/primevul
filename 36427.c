SYSCALL_DEFINE6(recvfrom, int, fd, void __user *, ubuf, size_t, size,
		unsigned, flags, struct sockaddr __user *, addr,
		int __user *, addr_len)
{
	struct socket *sock;
	struct iovec iov;
	struct msghdr msg;
	struct sockaddr_storage address;
	int err, err2;
	int fput_needed;

	if (size > INT_MAX)
		size = INT_MAX;
	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (!sock)
		goto out;

	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_iovlen = 1;
	msg.msg_iov = &iov;
	iov.iov_len = size;
	iov.iov_base = ubuf;
	msg.msg_name = (struct sockaddr *)&address;
	msg.msg_namelen = sizeof(address);
	if (sock->file->f_flags & O_NONBLOCK)
		flags |= MSG_DONTWAIT;
	err = sock_recvmsg(sock, &msg, size, flags);

	if (err >= 0 && addr != NULL) {
		err2 = move_addr_to_user((struct sockaddr *)&address,
					 msg.msg_namelen, addr, addr_len);
		if (err2 < 0)
			err = err2;
	}

	fput_light(sock->file, fput_needed);
out:
	return err;
}
