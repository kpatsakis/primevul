void kvm_apic_update_irr(struct kvm_vcpu *vcpu, u32 *pir)
{
	u32 i, pir_val;
	struct kvm_lapic *apic = vcpu->arch.apic;

	for (i = 0; i <= 7; i++) {
		pir_val = xchg(&pir[i], 0);
		if (pir_val)
			*((u32 *)(apic->regs + APIC_IRR + i * 0x10)) |= pir_val;
	}
}
