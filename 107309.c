int net_send(int s, int command, void *arg, int len)
{
	struct net_hdr *pnh;
	char *pktbuf;
	size_t pktlen;

	pktlen = sizeof(struct net_hdr) + len;

	pktbuf = (char*)calloc(sizeof(char), pktlen);
	if (pktbuf == NULL) {
		perror("calloc");
		goto net_send_error;
	}

	pnh = (struct net_hdr*)pktbuf;
	pnh->nh_type = command;
	pnh->nh_len = htonl(len);

	memcpy(pktbuf + sizeof(struct net_hdr), arg, len);

	for (;;) {
		ssize_t rc = send(s, pktbuf, pktlen, 0);

		if ((size_t)rc == pktlen)
			break;

		if (rc == EAGAIN || rc == EWOULDBLOCK || rc == EINTR)
			continue;

		if (rc == ECONNRESET)
			printf("Connection reset while sending packet!\n");

		goto net_send_error;
	}

	free(pktbuf);
	return 0;

net_send_error:
	free(pktbuf);
	return -1;
}
