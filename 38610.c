u64 kvm_lapic_get_cr8(struct kvm_vcpu *vcpu)
{
	u64 tpr;

	if (!kvm_vcpu_has_lapic(vcpu))
		return 0;

	tpr = (u64) kvm_apic_get_reg(vcpu->arch.apic, APIC_TASKPRI);

	return (tpr & 0xf0) >> 4;
}
