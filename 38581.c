static void apic_update_ppr(struct kvm_lapic *apic)
{
	u32 tpr, isrv, ppr, old_ppr;
	int isr;

	old_ppr = kvm_apic_get_reg(apic, APIC_PROCPRI);
	tpr = kvm_apic_get_reg(apic, APIC_TASKPRI);
	isr = apic_find_highest_isr(apic);
	isrv = (isr != -1) ? isr : 0;

	if ((tpr & 0xf0) >= (isrv & 0xf0))
		ppr = tpr & 0xff;
	else
		ppr = isrv & 0xf0;

	apic_debug("vlapic %p, ppr 0x%x, isr 0x%x, isrv 0x%x",
		   apic, ppr, isr, isrv);

	if (old_ppr != ppr) {
		apic_set_reg(apic, APIC_PROCPRI, ppr);
		if (ppr < old_ppr)
			kvm_make_request(KVM_REQ_EVENT, apic->vcpu);
	}
}
