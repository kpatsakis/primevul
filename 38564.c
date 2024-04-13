static inline int apic_lvtt_tscdeadline(struct kvm_lapic *apic)
{
	return ((kvm_apic_get_reg(apic, APIC_LVTT) &
		apic->lapic_timer.timer_mode_mask) ==
			APIC_LVT_TIMER_TSCDEADLINE);
}
