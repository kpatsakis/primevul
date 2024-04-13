static enum hrtimer_restart apic_timer_fn(struct hrtimer *data)
{
	struct kvm_timer *ktimer = container_of(data, struct kvm_timer, timer);
	struct kvm_lapic *apic = container_of(ktimer, struct kvm_lapic, lapic_timer);
	struct kvm_vcpu *vcpu = apic->vcpu;
	wait_queue_head_t *q = &vcpu->wq;

	/*
	 * There is a race window between reading and incrementing, but we do
	 * not care about potentially losing timer events in the !reinject
	 * case anyway. Note: KVM_REQ_PENDING_TIMER is implicitly checked
	 * in vcpu_enter_guest.
	 */
	if (!atomic_read(&ktimer->pending)) {
		atomic_inc(&ktimer->pending);
		/* FIXME: this code should not know anything about vcpus */
		kvm_make_request(KVM_REQ_PENDING_TIMER, vcpu);
	}

	if (waitqueue_active(q))
		wake_up_interruptible(q);

	if (lapic_is_periodic(apic)) {
		hrtimer_add_expires_ns(&ktimer->timer, ktimer->period);
		return HRTIMER_RESTART;
	} else
		return HRTIMER_NORESTART;
}
