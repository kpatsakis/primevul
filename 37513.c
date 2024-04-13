perf_event_nmi_handler(struct notifier_block *self,
			 unsigned long cmd, void *__args)
{
	struct die_args *args = __args;
	unsigned int this_nmi;
	int handled;

	if (!atomic_read(&active_events))
		return NOTIFY_DONE;

	switch (cmd) {
	case DIE_NMI:
		break;
	case DIE_NMIUNKNOWN:
		this_nmi = percpu_read(irq_stat.__nmi_count);
		if (this_nmi != __this_cpu_read(pmu_nmi.marked))
			/* let the kernel handle the unknown nmi */
			return NOTIFY_DONE;
		/*
		 * This one is a PMU back-to-back nmi. Two events
		 * trigger 'simultaneously' raising two back-to-back
		 * NMIs. If the first NMI handles both, the latter
		 * will be empty and daze the CPU. So, we drop it to
		 * avoid false-positive 'unknown nmi' messages.
		 */
		return NOTIFY_STOP;
	default:
		return NOTIFY_DONE;
	}

	apic_write(APIC_LVTPC, APIC_DM_NMI);

	handled = x86_pmu.handle_irq(args->regs);
	if (!handled)
		return NOTIFY_DONE;

	this_nmi = percpu_read(irq_stat.__nmi_count);
	if ((handled > 1) ||
		/* the next nmi could be a back-to-back nmi */
	    ((__this_cpu_read(pmu_nmi.marked) == this_nmi) &&
	     (__this_cpu_read(pmu_nmi.handled) > 1))) {
		/*
		 * We could have two subsequent back-to-back nmis: The
		 * first handles more than one counter, the 2nd
		 * handles only one counter and the 3rd handles no
		 * counter.
		 *
		 * This is the 2nd nmi because the previous was
		 * handling more than one counter. We will mark the
		 * next (3rd) and then drop it if unhandled.
		 */
		__this_cpu_write(pmu_nmi.marked, this_nmi + 1);
		__this_cpu_write(pmu_nmi.handled, handled);
	}

	return NOTIFY_STOP;
}
