int rawexpread_fully(off_t a, char *buf, size_t len, CLIENT *client) {
	ssize_t ret=0;

	while(len > 0 && (ret=rawexpread(a, buf, len, client)) > 0 ) {
		a += ret;
		buf += ret;
		len -= ret;
	}
	return (ret < 0 || len != 0);
}
