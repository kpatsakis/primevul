void kvm_lapic_set_eoi(struct kvm_vcpu *vcpu)
{
	if (kvm_vcpu_has_lapic(vcpu))
		apic_reg_write(vcpu->arch.apic, APIC_EOI, 0);
}
