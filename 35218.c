void open_modern(void) {
	struct addrinfo hints;
	struct addrinfo* ai = NULL;
	struct sock_flags;
	int e;

	memset(&hints, '\0', sizeof(hints));
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_protocol = IPPROTO_TCP;
	e = getaddrinfo(modern_listen, NBD_DEFAULT_PORT, &hints, &ai);
	if(e != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(e));
		exit(EXIT_FAILURE);
	}
	if((modernsock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol))<0) {
		err("socket: %m");
	}

	dosockopts(modernsock);

	if(bind(modernsock, ai->ai_addr, ai->ai_addrlen)) {
		err("bind: %m");
	}
	if(listen(modernsock, 10) <0) {
		err("listen: %m");
	}

	freeaddrinfo(ai);
}
