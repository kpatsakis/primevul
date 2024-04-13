void br_multicast_del_port(struct net_bridge_port *port)
{
	del_timer_sync(&port->multicast_router_timer);
}
