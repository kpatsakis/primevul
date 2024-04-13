static int do_net_open(char *iface)
{
	int s, port;
	char ip[16];
	struct sockaddr_in s_in;

	port = get_ip_port(iface, ip, sizeof(ip)-1);
	if (port == -1)
		return -1;

	s_in.sin_family = PF_INET;
	s_in.sin_port = htons(port);
	if (!inet_aton(ip, &s_in.sin_addr))
		return -1;

	if ((s = socket(s_in.sin_family, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return -1;

	printf("Connecting to %s port %d...\n", ip, port);

	if (connect(s, (struct sockaddr*) &s_in, sizeof(s_in)) == -1) {
		close(s);

		printf("Failed to connect\n");

		return -1;
	}

	if (handshake(s) == -1) {
		close(s);

		printf("Failed to connect - handshake failed\n");

		return -1;
	}

	printf("Connection successful\n");

	return s;
}
