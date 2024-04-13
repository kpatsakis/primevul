static int ctdb_tcp_get_address(struct ctdb_context *ctdb,
				const char *address, ctdb_sock_addr *addr)
{
	if (parse_ip(address, NULL, 0, addr) == 0) {
		DEBUG(DEBUG_CRIT, (__location__ " Unparsable address : %s.\n", address));
		return -1;
	}
	return 0;
}
