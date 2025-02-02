void dosockopts(int socket) {
#ifndef sun
	int yes=1;
#else
	char yes='1';
#endif /* sun */
	int sock_flags;

	/* lose the pesky "Address already in use" error message */
	if (setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
	        err("setsockopt SO_REUSEADDR");
	}
	if (setsockopt(socket,SOL_SOCKET,SO_KEEPALIVE,&yes,sizeof(int)) == -1) {
		err("setsockopt SO_KEEPALIVE");
	}

	/* make the listening socket non-blocking */
	if ((sock_flags = fcntl(socket, F_GETFL, 0)) == -1) {
		err("fcntl F_GETFL");
	}
	if (fcntl(socket, F_SETFL, sock_flags | O_NONBLOCK) == -1) {
		err("fcntl F_SETFL O_NONBLOCK");
	}
}
