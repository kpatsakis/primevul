static inline int apic_lvt_enabled(struct kvm_lapic *apic, int lvt_type)
{
	return !(kvm_apic_get_reg(apic, lvt_type) & APIC_LVT_MASKED);
}
