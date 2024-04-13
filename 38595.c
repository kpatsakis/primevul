static inline void kvm_apic_set_id(struct kvm_lapic *apic, u8 id)
{
	apic_set_reg(apic, APIC_ID, id << 24);
	recalculate_apic_map(apic->vcpu->kvm);
}
