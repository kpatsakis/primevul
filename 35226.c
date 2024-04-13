void set_peername(int net, CLIENT *client) {
	struct sockaddr_storage addrin;
	struct sockaddr_storage netaddr;
	struct sockaddr_in  *netaddr4 = NULL;
	struct sockaddr_in6 *netaddr6 = NULL;
	size_t addrinlen = sizeof( addrin );
	struct addrinfo hints;
	struct addrinfo *ai = NULL;
	char peername[NI_MAXHOST];
	char netname[NI_MAXHOST];
	char *tmp = NULL;
	int i;
	int e;
	int shift;

	if (getpeername(net, (struct sockaddr *) &addrin, (socklen_t *)&addrinlen) < 0)
		err("getsockname failed: %m");

	getnameinfo((struct sockaddr *)&addrin, (socklen_t)addrinlen,
		peername, sizeof (peername), NULL, 0, NI_NUMERICHOST);

	memset(&hints, '\0', sizeof (hints));
	hints.ai_flags = AI_ADDRCONFIG;
	e = getaddrinfo(peername, NULL, &hints, &ai);

	if(e != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(e));
		freeaddrinfo(ai);
		return;
	}

	switch(client->server->virtstyle) {
		case VIRT_NONE:
			client->exportname=g_strdup(client->server->exportname);
			break;
		case VIRT_IPHASH:
			for(i=0;i<strlen(peername);i++) {
				if(peername[i]=='.') {
					peername[i]='/';
				}
			}
		case VIRT_IPLIT:
			client->exportname=g_strdup_printf(client->server->exportname, peername);
			break;
		case VIRT_CIDR:
			memcpy(&netaddr, &addrin, addrinlen);
			if(ai->ai_family == AF_INET) {
				netaddr4 = (struct sockaddr_in *)&netaddr;
				(netaddr4->sin_addr).s_addr>>=32-(client->server->cidrlen);
				(netaddr4->sin_addr).s_addr<<=32-(client->server->cidrlen);

				getnameinfo((struct sockaddr *) netaddr4, (socklen_t) addrinlen,
							netname, sizeof (netname), NULL, 0, NI_NUMERICHOST);
				tmp=g_strdup_printf("%s/%s", netname, peername);
			}else if(ai->ai_family == AF_INET6) {
				netaddr6 = (struct sockaddr_in6 *)&netaddr;

				shift = 128-(client->server->cidrlen);
				i = 3;
				while(shift >= 32) {
					((netaddr6->sin6_addr).s6_addr32[i])=0;
					shift-=32;
					i--;
				}
				(netaddr6->sin6_addr).s6_addr32[i]>>=shift;
				(netaddr6->sin6_addr).s6_addr32[i]<<=shift;

				getnameinfo((struct sockaddr *)netaddr6, (socklen_t)addrinlen,
					    netname, sizeof(netname), NULL, 0, NI_NUMERICHOST);
				tmp=g_strdup_printf("%s/%s", netname, peername);
			}

			if(tmp != NULL)
			  client->exportname=g_strdup_printf(client->server->exportname, tmp);

			break;
	}

	freeaddrinfo(ai);
	msg4(LOG_INFO, "connect from %s, assigned file is %s", 
	     peername, client->exportname);
	client->clientname=g_strdup(peername);
}
