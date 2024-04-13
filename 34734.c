void ctdb_tcp_stop_connection(struct ctdb_node *node)
{
	struct ctdb_tcp_node *tnode = talloc_get_type(
		node->private_data, struct ctdb_tcp_node);
	
	ctdb_queue_set_fd(tnode->out_queue, -1);
	talloc_free(tnode->connect_te);
	talloc_free(tnode->connect_fde);
	tnode->connect_fde = NULL;
	tnode->connect_te = NULL;
	if (tnode->fd != -1) {
		close(tnode->fd);
		tnode->fd = -1;
	}
}
