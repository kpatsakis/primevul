static void br_multicast_start_querier(struct net_bridge *br)
{
	struct net_bridge_port *port;

	br_multicast_open(br);

	list_for_each_entry(port, &br->port_list, list) {
		if (port->state == BR_STATE_DISABLED ||
		    port->state == BR_STATE_BLOCKING)
			continue;

		__br_multicast_enable_port(port);
	}
}
