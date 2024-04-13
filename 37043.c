mmtimer_interrupt(int irq, void *dev_id)
{
	unsigned long expires = 0;
	int result = IRQ_NONE;
	unsigned indx = cpu_to_node(smp_processor_id());
	struct mmtimer *base;

	spin_lock(&timers[indx].lock);
	base = rb_entry(timers[indx].next, struct mmtimer, list);
	if (base == NULL) {
		spin_unlock(&timers[indx].lock);
		return result;
	}

	if (base->cpu == smp_processor_id()) {
		if (base->timer)
			expires = base->timer->it.mmtimer.expires;
		/* expires test won't work with shared irqs */
		if ((mmtimer_int_pending(COMPARATOR) > 0) ||
			(expires && (expires <= rtc_time()))) {
			mmtimer_clr_int_pending(COMPARATOR);
			tasklet_schedule(&timers[indx].tasklet);
			result = IRQ_HANDLED;
		}
	}
	spin_unlock(&timers[indx].lock);
	return result;
}
