static long cap_prctl_drop(struct cred *new, unsigned long cap)
{
	if (!capable(CAP_SETPCAP))
		return -EPERM;
	if (!cap_valid(cap))
		return -EINVAL;

	cap_lower(new->cap_bset, cap);
	return 0;
}
