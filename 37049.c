static void mmtimer_setup_int_1(int cpu, u64 expires)
{
	u64 val;

	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC2_INT_ENABLE), 0UL);

	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_INT_CMPC), -1L);

	mmtimer_clr_int_pending(1);

	val = ((u64)SGI_MMTIMER_VECTOR << SH_RTC2_INT_CONFIG_IDX_SHFT) |
		((u64)cpu_physical_id(cpu) <<
			SH_RTC2_INT_CONFIG_PID_SHFT);

	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC2_INT_CONFIG), val);

	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC2_INT_ENABLE), 1UL);

	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_INT_CMPC), expires);
}
