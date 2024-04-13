static inline void apic_set_irr(int vec, struct kvm_lapic *apic)
{
	apic->irr_pending = true;
	apic_set_vector(vec, apic->regs + APIC_IRR);
}
