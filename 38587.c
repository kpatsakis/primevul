static inline int kvm_apic_id(struct kvm_lapic *apic)
{
 	return (kvm_apic_get_reg(apic, APIC_ID) >> 24) & 0xff;
 }
