static int mmtimer_disable_int(long nasid, int comparator)
{
	switch (comparator) {
	case 0:
		nasid == -1 ? HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_ENABLE),
			0UL) : REMOTE_HUB_S(nasid, SH_RTC1_INT_ENABLE, 0UL);
		break;
	case 1:
		nasid == -1 ? HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC2_INT_ENABLE),
			0UL) : REMOTE_HUB_S(nasid, SH_RTC2_INT_ENABLE, 0UL);
		break;
	case 2:
		nasid == -1 ? HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC3_INT_ENABLE),
			0UL) : REMOTE_HUB_S(nasid, SH_RTC3_INT_ENABLE, 0UL);
		break;
	default:
		return -EFAULT;
	}
	return 0;
}
