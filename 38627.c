static void start_apic_timer(struct kvm_lapic *apic)
{
	ktime_t now;
	atomic_set(&apic->lapic_timer.pending, 0);

	if (apic_lvtt_period(apic) || apic_lvtt_oneshot(apic)) {
		/* lapic timer in oneshot or periodic mode */
		now = apic->lapic_timer.timer.base->get_time();
		apic->lapic_timer.period = (u64)kvm_apic_get_reg(apic, APIC_TMICT)
			    * APIC_BUS_CYCLE_NS * apic->divide_count;

		if (!apic->lapic_timer.period)
			return;
		/*
		 * Do not allow the guest to program periodic timers with small
		 * interval, since the hrtimers are not throttled by the host
		 * scheduler.
		 */
		if (apic_lvtt_period(apic)) {
			s64 min_period = min_timer_period_us * 1000LL;

			if (apic->lapic_timer.period < min_period) {
				pr_info_ratelimited(
				    "kvm: vcpu %i: requested %lld ns "
				    "lapic timer period limited to %lld ns\n",
				    apic->vcpu->vcpu_id,
				    apic->lapic_timer.period, min_period);
				apic->lapic_timer.period = min_period;
			}
		}

		hrtimer_start(&apic->lapic_timer.timer,
			      ktime_add_ns(now, apic->lapic_timer.period),
			      HRTIMER_MODE_ABS);

		apic_debug("%s: bus cycle is %" PRId64 "ns, now 0x%016"
			   PRIx64 ", "
			   "timer initial count 0x%x, period %lldns, "
			   "expire @ 0x%016" PRIx64 ".\n", __func__,
			   APIC_BUS_CYCLE_NS, ktime_to_ns(now),
			   kvm_apic_get_reg(apic, APIC_TMICT),
			   apic->lapic_timer.period,
			   ktime_to_ns(ktime_add_ns(now,
					apic->lapic_timer.period)));
	} else if (apic_lvtt_tscdeadline(apic)) {
		/* lapic timer in tsc deadline mode */
		u64 guest_tsc, tscdeadline = apic->lapic_timer.tscdeadline;
		u64 ns = 0;
		struct kvm_vcpu *vcpu = apic->vcpu;
		unsigned long this_tsc_khz = vcpu->arch.virtual_tsc_khz;
		unsigned long flags;

		if (unlikely(!tscdeadline || !this_tsc_khz))
			return;

		local_irq_save(flags);

		now = apic->lapic_timer.timer.base->get_time();
		guest_tsc = kvm_x86_ops->read_l1_tsc(vcpu, native_read_tsc());
		if (likely(tscdeadline > guest_tsc)) {
			ns = (tscdeadline - guest_tsc) * 1000000ULL;
			do_div(ns, this_tsc_khz);
		}
		hrtimer_start(&apic->lapic_timer.timer,
			ktime_add_ns(now, ns), HRTIMER_MODE_ABS);

		local_irq_restore(flags);
	}
}
