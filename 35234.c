inline void writeit(int f, void *buf, size_t len) {
	ssize_t res;
	while (len > 0) {
		DEBUG("+");
		if ((res = write(f, buf, len)) <= 0)
			err("Send failed: %m");
		len -= res;
		buf += res;
	}
}
