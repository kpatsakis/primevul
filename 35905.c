static inline int cap_inh_is_capped(void)
{

	/* they are so limited unless the current task has the CAP_SETPCAP
	 * capability
	 */
	if (cap_capable(current_cred(), current_cred()->user->user_ns,
			CAP_SETPCAP, SECURITY_CAP_AUDIT) == 0)
		return 0;
	return 1;
}
