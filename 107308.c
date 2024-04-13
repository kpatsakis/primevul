int net_read_exact(int s, void *arg, int len)
{
	ssize_t rc;
	int rlen = 0;
	char *buf = (char*)arg;
	while (rlen < len) {
		rc = recv(s, buf, (len - rlen), 0);

		if (rc < 1) {
			if (rc == -1 && (errno == EAGAIN || errno == EINTR)) {
				usleep(100);
				continue;
			}

			return -1;
		}

		buf += rc;
		rlen += rc;
	}

	return 0;
}
