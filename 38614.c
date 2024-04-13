void kvm_lapic_set_tpr(struct kvm_vcpu *vcpu, unsigned long cr8)
{
	struct kvm_lapic *apic = vcpu->arch.apic;

	if (!kvm_vcpu_has_lapic(vcpu))
		return;

	apic_set_tpr(apic, ((cr8 & 0x0f) << 4)
		     | (kvm_apic_get_reg(apic, APIC_TASKPRI) & 4));
}
