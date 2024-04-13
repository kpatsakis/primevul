static int get_ip_port(char *iface, char *ip, const int ipsize)
{
	char *host;
	char *ptr;
	int port = -1;
	struct in_addr addr;

	host = strdup(iface);
	if (!host)
		return -1;

	ptr = strchr(host, ':');
	if (!ptr)
		goto out;

	*ptr++ = 0;

	if (!inet_aton(host, &addr))
		goto out; /* XXX resolve hostname */

	assert(strlen(host) <= 15);
	strncpy(ip, host, ipsize);
	port = atoi(ptr);

out:
	free(host);
	return port;
}
