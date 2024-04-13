send_cancel(struct TCP_Server_Info *server, void *buf, struct mid_q_entry *mid)
{
	return server->ops->send_cancel ?
				server->ops->send_cancel(server, buf, mid) : 0;
}
