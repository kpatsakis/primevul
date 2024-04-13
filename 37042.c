static int mmtimer_int_pending(int comparator)
{
	if (HUB_L((unsigned long *)LOCAL_MMR_ADDR(SH_EVENT_OCCURRED)) &
			SH_EVENT_OCCURRED_RTC1_INT_MASK << comparator)
		return 1;
	else
		return 0;
}
