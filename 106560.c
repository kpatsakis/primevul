checkcondition_arr(void *checkval, ITEM *item)
{
	int32	   *StopLow = ((CHKVAL *) checkval)->arrb;
	int32	   *StopHigh = ((CHKVAL *) checkval)->arre;
	int32	   *StopMiddle;

	/* Loop invariant: StopLow <= val < StopHigh */

	while (StopLow < StopHigh)
	{
		StopMiddle = StopLow + (StopHigh - StopLow) / 2;
		if (*StopMiddle == item->val)
			return (true);
		else if (*StopMiddle < item->val)
			StopLow = StopMiddle + 1;
		else
			StopHigh = StopMiddle;
	}
	return false;
}
