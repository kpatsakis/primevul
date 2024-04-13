static inline void apic_clear_isr(int vec, struct kvm_lapic *apic)
{
	if (__apic_test_and_clear_vector(vec, apic->regs + APIC_ISR))
		--apic->isr_count;
	BUG_ON(apic->isr_count < 0);
	apic->highest_isr_cache = -1;
}
