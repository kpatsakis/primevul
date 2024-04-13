CLIENT* negotiate(int net, CLIENT *client, GArray* servers) {
	char zeros[128];
	uint64_t size_host;
	uint32_t flags = NBD_FLAG_HAS_FLAGS;
	uint16_t smallflags = 0;
	uint64_t magic;

	memset(zeros, '\0', sizeof(zeros));
	if(!client || !client->modern) {
		/* common */
		if (write(net, INIT_PASSWD, 8) < 0) {
			err_nonfatal("Negotiation failed: %m");
			if(client)
				exit(EXIT_FAILURE);
		}
		if(!client || client->modern) {
			/* modern */
			magic = htonll(opts_magic);
		} else {
			/* oldstyle */
			magic = htonll(cliserv_magic);
		}
		if (write(net, &magic, sizeof(magic)) < 0) {
			err_nonfatal("Negotiation failed: %m");
			if(client)
				exit(EXIT_FAILURE);
		}
	}
	if(!client) {
		/* modern */
		uint32_t reserved;
		uint32_t opt;
		uint32_t namelen;
		char* name;
		int i;

		if(!servers)
			err("programmer error");
		if (write(net, &smallflags, sizeof(uint16_t)) < 0)
			err("Negotiation failed: %m");
		if (read(net, &reserved, sizeof(reserved)) < 0)
			err("Negotiation failed: %m");
		if (read(net, &magic, sizeof(magic)) < 0)
			err("Negotiation failed: %m");
		magic = ntohll(magic);
		if(magic != opts_magic) {
			close(net);
			return NULL;
		}
		if (read(net, &opt, sizeof(opt)) < 0)
			err("Negotiation failed: %m");
		opt = ntohl(opt);
		if(opt != NBD_OPT_EXPORT_NAME) {
			close(net);
			return NULL;
		}
		if (read(net, &namelen, sizeof(namelen)) < 0)
			err("Negotiation failed: %m");
		namelen = ntohl(namelen);
		name = malloc(namelen+1);
		name[namelen]=0;
		if (read(net, name, namelen) < 0)
			err("Negotiation failed: %m");
		for(i=0; i<servers->len; i++) {
			SERVER* serve = &(g_array_index(servers, SERVER, i));
			if(!strcmp(serve->servename, name)) {
				CLIENT* client = g_new0(CLIENT, 1);
				client->server = serve;
				client->exportsize = OFFT_MAX;
				client->net = net;
				client->modern = TRUE;
				return client;
			}
		}
		return NULL;
	}
	/* common */
	size_host = htonll((u64)(client->exportsize));
	if (write(net, &size_host, 8) < 0)
		err("Negotiation failed: %m");
	if (client->server->flags & F_READONLY)
		flags |= NBD_FLAG_READ_ONLY;
	if (!client->modern) {
		/* oldstyle */
		flags = htonl(flags);
		if (write(client->net, &flags, 4) < 0)
			err("Negotiation failed: %m");
	} else {
		/* modern */
		smallflags = (uint16_t)(flags & ~((uint16_t)0));
		smallflags = htons(smallflags);
		if (write(client->net, &smallflags, sizeof(smallflags)) < 0) {
			err("Negotiation failed: %m");
		}
	}
	/* common */
	if (write(client->net, zeros, 124) < 0)
		err("Negotiation failed: %m");
	return NULL;
}
