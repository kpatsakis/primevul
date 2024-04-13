static int sgi_timer_del(struct k_itimer *timr)
{
	cnodeid_t nodeid = timr->it.mmtimer.node;
	unsigned long irqflags;

	spin_lock_irqsave(&timers[nodeid].lock, irqflags);
	if (timr->it.mmtimer.clock != TIMER_OFF) {
		unsigned long expires = timr->it.mmtimer.expires;
		struct rb_node *n = timers[nodeid].timer_head.rb_node;
		struct mmtimer *uninitialized_var(t);
		int r = 0;

		timr->it.mmtimer.clock = TIMER_OFF;
		timr->it.mmtimer.expires = 0;

		while (n) {
			t = rb_entry(n, struct mmtimer, list);
			if (t->timer == timr)
				break;

			if (expires < t->timer->it.mmtimer.expires)
				n = n->rb_left;
			else
				n = n->rb_right;
		}

		if (!n) {
			spin_unlock_irqrestore(&timers[nodeid].lock, irqflags);
			return 0;
		}

		if (timers[nodeid].next == n) {
			timers[nodeid].next = rb_next(n);
			r = 1;
		}

		rb_erase(n, &timers[nodeid].timer_head);
		kfree(t);

		if (r) {
			mmtimer_disable_int(cnodeid_to_nasid(nodeid),
				COMPARATOR);
			mmtimer_set_next_timer(nodeid);
		}
	}
	spin_unlock_irqrestore(&timers[nodeid].lock, irqflags);
 	return 0;
 }
