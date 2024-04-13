timeval_to_jiffies(const struct timeval *value)
{
	unsigned long sec = value->tv_sec;
	long usec = value->tv_usec;

	if (sec >= MAX_SEC_IN_JIFFIES){
		sec = MAX_SEC_IN_JIFFIES;
		usec = 0;
	}
	return (((u64)sec * SEC_CONVERSION) +
		(((u64)usec * USEC_CONVERSION + USEC_ROUND) >>
		 (USEC_JIFFIE_SC - SEC_JIFFIE_SC))) >> SEC_JIFFIE_SC;
}
