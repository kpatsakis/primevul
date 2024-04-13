static void ctdb_listen_event(struct event_context *ev, struct fd_event *fde, 
			      uint16_t flags, void *private_data)
{
	struct ctdb_context *ctdb = talloc_get_type(private_data, struct ctdb_context);
	struct ctdb_tcp *ctcp = talloc_get_type(ctdb->private_data, struct ctdb_tcp);
	ctdb_sock_addr addr;
	socklen_t len;
	int fd, nodeid;
	struct ctdb_incoming *in;
	int one = 1;
	const char *incoming_node;

	memset(&addr, 0, sizeof(addr));
	len = sizeof(addr);
	fd = accept(ctcp->listen_fd, (struct sockaddr *)&addr, &len);
	if (fd == -1) return;

	incoming_node = ctdb_addr_to_str(&addr);
	nodeid = ctdb_ip_to_nodeid(ctdb, incoming_node);

	if (nodeid == -1) {
		DEBUG(DEBUG_ERR, ("Refused connection from unknown node %s\n", incoming_node));
		close(fd);
		return;
	}

	in = talloc_zero(ctcp, struct ctdb_incoming);
	in->fd = fd;
	in->ctdb = ctdb;

	set_nonblocking(in->fd);
	set_close_on_exec(in->fd);

	DEBUG(DEBUG_DEBUG, (__location__ " Created SOCKET FD:%d to incoming ctdb connection\n", fd));

        setsockopt(in->fd,SOL_SOCKET,SO_KEEPALIVE,(char *)&one,sizeof(one));

	in->queue = ctdb_queue_setup(ctdb, in, in->fd, CTDB_TCP_ALIGNMENT, 
				     ctdb_tcp_read_cb, in, "ctdbd-%s", incoming_node);
}
