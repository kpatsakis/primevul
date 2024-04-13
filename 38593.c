void kvm_apic_post_state_restore(struct kvm_vcpu *vcpu,
		struct kvm_lapic_state *s)
{
	struct kvm_lapic *apic = vcpu->arch.apic;

	kvm_lapic_set_base(vcpu, vcpu->arch.apic_base);
	/* set SPIV separately to get count of SW disabled APICs right */
	apic_set_spiv(apic, *((u32 *)(s->regs + APIC_SPIV)));
	memcpy(vcpu->arch.apic->regs, s->regs, sizeof *s);
	/* call kvm_apic_set_id() to put apic into apic_map */
	kvm_apic_set_id(apic, kvm_apic_id(apic));
	kvm_apic_set_version(vcpu);

	apic_update_ppr(apic);
	hrtimer_cancel(&apic->lapic_timer.timer);
	update_divide_count(apic);
	start_apic_timer(apic);
	apic->irr_pending = true;
	apic->isr_count = kvm_apic_vid_enabled(vcpu->kvm) ?
				1 : count_vectors(apic->regs + APIC_ISR);
	apic->highest_isr_cache = -1;
	kvm_x86_ops->hwapic_isr_update(vcpu->kvm, apic_find_highest_isr(apic));
	kvm_make_request(KVM_REQ_EVENT, vcpu);
	kvm_rtc_eoi_tracking_restore_one(vcpu);
}
