static void mmtimer_add_list(struct mmtimer *n)
{
	int nodeid = n->timer->it.mmtimer.node;
	unsigned long expires = n->timer->it.mmtimer.expires;
	struct rb_node **link = &timers[nodeid].timer_head.rb_node;
	struct rb_node *parent = NULL;
	struct mmtimer *x;

	/*
	 * Find the right place in the rbtree:
	 */
	while (*link) {
		parent = *link;
		x = rb_entry(parent, struct mmtimer, list);

		if (expires < x->timer->it.mmtimer.expires)
			link = &(*link)->rb_left;
		else
			link = &(*link)->rb_right;
	}

	/*
	 * Insert the timer to the rbtree and check whether it
	 * replaces the first pending timer
	 */
	rb_link_node(&n->list, parent, link);
	rb_insert_color(&n->list, &timers[nodeid].timer_head);

	if (!timers[nodeid].next || expires < rb_entry(timers[nodeid].next,
			struct mmtimer, list)->timer->it.mmtimer.expires)
		timers[nodeid].next = &n->list;
}
