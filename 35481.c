static int ep_poll_wakeup_proc(void *priv, void *cookie, int call_nests)
{
	ep_wake_up_nested((wait_queue_head_t *) cookie, POLLIN,
			  1 + call_nests);
	return 0;
}
