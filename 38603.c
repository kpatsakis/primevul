int kvm_get_apic_interrupt(struct kvm_vcpu *vcpu)
{
	int vector = kvm_apic_has_interrupt(vcpu);
	struct kvm_lapic *apic = vcpu->arch.apic;

	if (vector == -1)
		return -1;

	apic_set_isr(vector, apic);
	apic_update_ppr(apic);
	apic_clear_irr(vector, apic);
	return vector;
}
