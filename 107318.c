void handle_dude(int dude, int udp)
{
	unsigned char buf[2048];
	int rc;
	fd_set rfds;
	int maxfd;
	struct sockaddr_in s_in;
	socklen_t len;

	/* handshake */
	rc = recv(dude, buf, 5, 0);
	if (rc != 5) {
		close(dude);
		return;
	}

	if (memcmp(buf, "sorbo", 5) != 0) {
		close(dude);
		return;
	}

	if (send(dude, "sorbox", 6, 0) != 6) {
		close(dude);
		return;
	}

	printf("Handshake complete\n");
	memset(ids, 0, sizeof(ids));
	last_id = 0;
	wrap = 0;

	while (1) {
		FD_ZERO(&rfds);
		FD_SET(udp, &rfds);
		FD_SET(dude, &rfds);

		if (dude > udp)
			maxfd = dude;
		else
			maxfd = udp;

		if (select(maxfd+1, &rfds, NULL, NULL, NULL) == -1)
			err(1, "select()");

		if (FD_ISSET(dude, &rfds))
			break;

		if (!FD_ISSET(udp, &rfds))
			continue;

		len = sizeof(s_in);
		rc = recvfrom(udp, buf, sizeof(buf), 0,
			      (struct sockaddr*) &s_in, &len);
		if (rc == -1)
			err(1, "read()");

		if (handle(dude, buf, rc, &s_in))
			break;
	}
	close(dude);
}
