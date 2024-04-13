static inline void kvm_apic_set_ldr(struct kvm_lapic *apic, u32 id)
{
	apic_set_reg(apic, APIC_LDR, id);
	recalculate_apic_map(apic->vcpu->kvm);
}
