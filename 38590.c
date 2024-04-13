int kvm_apic_match_physical_addr(struct kvm_lapic *apic, u16 dest)
{
	return dest == 0xff || kvm_apic_id(apic) == dest;
}
