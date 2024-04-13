static void kvm_ioapic_send_eoi(struct kvm_lapic *apic, int vector)
{
	if (!(kvm_apic_get_reg(apic, APIC_SPIV) & APIC_SPIV_DIRECTED_EOI) &&
	    kvm_ioapic_handles_vector(apic->vcpu->kvm, vector)) {
		int trigger_mode;
		if (apic_test_vector(vector, apic->regs + APIC_TMR))
			trigger_mode = IOAPIC_LEVEL_TRIG;
		else
			trigger_mode = IOAPIC_EDGE_TRIG;
		kvm_ioapic_update_eoi(apic->vcpu, vector, trigger_mode);
	}
}
