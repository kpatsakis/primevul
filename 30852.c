int parse_counters(const char *string, struct xt_counters *ctr)
{
	int ret;

	if (!string)
		return 0;

	ret = sscanf(string, "[%llu:%llu]",
		     (unsigned long long *)&ctr->pcnt,
		     (unsigned long long *)&ctr->bcnt);

	return ret == 2;
}
