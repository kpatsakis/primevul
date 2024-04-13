void kvm_apic_update_tmr(struct kvm_vcpu *vcpu, u32 *tmr)
{
	struct kvm_lapic *apic = vcpu->arch.apic;
	int i;

	for (i = 0; i < 8; i++)
		apic_set_reg(apic, APIC_TMR + 0x10 * i, tmr[i]);
}
