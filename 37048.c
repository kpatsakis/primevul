static void mmtimer_setup_int_0(int cpu, u64 expires)
{
	u64 val;

	/* Disable interrupt */
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_ENABLE), 0UL);

	/* Initialize comparator value */
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_INT_CMPB), -1L);

	/* Clear pending bit */
	mmtimer_clr_int_pending(0);

	val = ((u64)SGI_MMTIMER_VECTOR << SH_RTC1_INT_CONFIG_IDX_SHFT) |
		((u64)cpu_physical_id(cpu) <<
			SH_RTC1_INT_CONFIG_PID_SHFT);

	/* Set configuration */
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_CONFIG), val);

	/* Enable RTC interrupts */
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_ENABLE), 1UL);

	/* Initialize comparator value */
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_INT_CMPB), expires);


}
