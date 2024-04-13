static inline int ep_op_has_event(int op)
{
	return op != EPOLL_CTL_DEL;
}
