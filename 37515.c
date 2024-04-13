static inline int x86_pmu_addr_offset(int index)
{
	if (boot_cpu_has(X86_FEATURE_PERFCTR_CORE))
		return index << 1;
	return index;
}
