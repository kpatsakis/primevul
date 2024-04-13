void ctdb_tcp_tnode_cb(uint8_t *data, size_t cnt, void *private_data)
{
	struct ctdb_node *node = talloc_get_type(private_data, struct ctdb_node);
	struct ctdb_tcp_node *tnode = talloc_get_type(
		node->private_data, struct ctdb_tcp_node);

	if (data == NULL) {
		node->ctdb->upcalls->node_dead(node);
	}

	ctdb_tcp_stop_connection(node);
	tnode->connect_te = event_add_timed(node->ctdb->ev, tnode,
					    timeval_current_ofs(3, 0),
					    ctdb_tcp_node_connect, node);
}
