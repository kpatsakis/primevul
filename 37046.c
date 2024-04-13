static void mmtimer_set_next_timer(int nodeid)
{
	struct mmtimer_node *n = &timers[nodeid];
	struct mmtimer *x;
	struct k_itimer *t;
	int o;

restart:
	if (n->next == NULL)
		return;

	x = rb_entry(n->next, struct mmtimer, list);
	t = x->timer;
	if (!t->it.mmtimer.incr) {
		/* Not an interval timer */
		if (!mmtimer_setup(x->cpu, COMPARATOR,
					t->it.mmtimer.expires)) {
			/* Late setup, fire now */
			tasklet_schedule(&n->tasklet);
		}
		return;
	}

	/* Interval timer */
	o = 0;
	while (!mmtimer_setup(x->cpu, COMPARATOR, t->it.mmtimer.expires)) {
		unsigned long e, e1;
		struct rb_node *next;
		t->it.mmtimer.expires += t->it.mmtimer.incr << o;
		t->it_overrun += 1 << o;
		o++;
		if (o > 20) {
			printk(KERN_ALERT "mmtimer: cannot reschedule timer\n");
			t->it.mmtimer.clock = TIMER_OFF;
			n->next = rb_next(&x->list);
			rb_erase(&x->list, &n->timer_head);
			kfree(x);
			goto restart;
		}

		e = t->it.mmtimer.expires;
		next = rb_next(&x->list);

		if (next == NULL)
			continue;

		e1 = rb_entry(next, struct mmtimer, list)->
			timer->it.mmtimer.expires;
		if (e > e1) {
			n->next = next;
			rb_erase(&x->list, &n->timer_head);
			mmtimer_add_list(x);
			goto restart;
		}
	}
}
