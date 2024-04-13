static inline void apic_clear_irr(int vec, struct kvm_lapic *apic)
{
	apic->irr_pending = false;
	apic_clear_vector(vec, apic->regs + APIC_IRR);
	if (apic_search_irr(apic) != -1)
		apic->irr_pending = true;
}
