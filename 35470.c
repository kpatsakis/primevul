static inline int ep_is_linked(struct list_head *p)
{
	return !list_empty(p);
}
