wait_for_free_request(struct TCP_Server_Info *server, const int timeout,
		      const int optype)
{
	return wait_for_free_credits(server, timeout,
				server->ops->get_credits_field(server, optype));
}
