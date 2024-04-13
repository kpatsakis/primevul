int kvm_apic_match_logical_addr(struct kvm_lapic *apic, u8 mda)
{
	int result = 0;
	u32 logical_id;

	if (apic_x2apic_mode(apic)) {
		logical_id = kvm_apic_get_reg(apic, APIC_LDR);
		return logical_id & mda;
	}

	logical_id = GET_APIC_LOGICAL_ID(kvm_apic_get_reg(apic, APIC_LDR));

	switch (kvm_apic_get_reg(apic, APIC_DFR)) {
	case APIC_DFR_FLAT:
		if (logical_id & mda)
			result = 1;
		break;
	case APIC_DFR_CLUSTER:
		if (((logical_id >> 4) == (mda >> 0x4))
		    && (logical_id & mda & 0xf))
			result = 1;
		break;
	default:
		apic_debug("Bad DFR vcpu %d: %08x\n",
			   apic->vcpu->vcpu_id, kvm_apic_get_reg(apic, APIC_DFR));
		break;
	}

	return result;
}
