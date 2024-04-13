static bool check_hw_exists(void)
{
	u64 val, val_new = 0;
	int i, reg, ret = 0;

	/*
	 * Check to see if the BIOS enabled any of the counters, if so
	 * complain and bail.
	 */
	for (i = 0; i < x86_pmu.num_counters; i++) {
		reg = x86_pmu_config_addr(i);
		ret = rdmsrl_safe(reg, &val);
		if (ret)
			goto msr_fail;
		if (val & ARCH_PERFMON_EVENTSEL_ENABLE)
			goto bios_fail;
	}

	if (x86_pmu.num_counters_fixed) {
		reg = MSR_ARCH_PERFMON_FIXED_CTR_CTRL;
		ret = rdmsrl_safe(reg, &val);
		if (ret)
			goto msr_fail;
		for (i = 0; i < x86_pmu.num_counters_fixed; i++) {
			if (val & (0x03 << i*4))
				goto bios_fail;
		}
	}

	/*
	 * Now write a value and read it back to see if it matches,
	 * this is needed to detect certain hardware emulators (qemu/kvm)
	 * that don't trap on the MSR access and always return 0s.
	 */
	val = 0xabcdUL;
	ret = checking_wrmsrl(x86_pmu_event_addr(0), val);
	ret |= rdmsrl_safe(x86_pmu_event_addr(0), &val_new);
	if (ret || val != val_new)
		goto msr_fail;

	return true;

bios_fail:
	printk(KERN_CONT "Broken BIOS detected, using software events only.\n");
	printk(KERN_ERR FW_BUG "the BIOS has corrupted hw-PMU resources (MSR %x is %Lx)\n", reg, val);
	return false;

msr_fail:
	printk(KERN_CONT "Broken PMU hardware detected, using software events only.\n");
	return false;
}
