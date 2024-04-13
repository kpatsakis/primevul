int main(int argc, char *argv[])
{
	struct utsname utsName;
	struct sockaddr_in s_in;
	struct sockaddr_in dude_sin;
	int len, udp, ch, dude, s;
	int port = S_DEFAULT_PORT;
	int drop;

	while ((ch = getopt(argc, argv, "ph")) != -1) {
		switch (ch) {
		case 'p':
			drop = 0;
			break;

		default:
		case 'h':
			usage();
			break;

		}
	}

	memset(&s_in, 0, sizeof(s_in));
	s_in.sin_family = PF_INET;
	s_in.sin_addr.s_addr = INADDR_ANY;
	s_in.sin_port = htons(S_DEFAULT_UDP_PORT);

	udp = socket(s_in.sin_family, SOCK_DGRAM, IPPROTO_UDP);
	if (udp == -1)
		err(1, "socket(UDP)");
	if (bind(udp, (struct sockaddr*) &s_in, sizeof(s_in)) == -1)
		err(1, "bind()");

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == -1)
		err(1, "socket(TCP)");

	drop = 1;

	if (uname(&utsName) == 0)
	{
		drop = strncasecmp(utsName.sysname, "cygwin", 6);
	}

	if (drop)
		drop_privs();

	memset(&s_in, 0, sizeof(s_in));
	s_in.sin_family = PF_INET;
	s_in.sin_port = htons(port);
	s_in.sin_addr.s_addr = INADDR_ANY;

	len = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &len, sizeof(len)) == -1)
		err(1, "setsockopt(SO_REUSEADDR)");

	if (bind(s, (struct sockaddr*) &s_in, sizeof(s_in)) == -1)
		err(1, "bind()");

	if (listen(s, 5) == -1)
		err(1, "listen()");


	while (1) {
		len = sizeof(dude_sin);
		printf("Waiting for connexion\n");
		dude = accept(s, (struct sockaddr*) &dude_sin,
			      (socklen_t*) &len);
		if (dude == -1)
			err(1, "accept()");

		printf("Got connection from %s\n",
		       inet_ntoa(dude_sin.sin_addr));
		handle_dude(dude, udp);
		printf("That was it\n");
	}
	exit(0);
}
