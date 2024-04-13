int cap_capable(const struct cred *cred, struct user_namespace *targ_ns,
		int cap, int audit)
{
	for (;;) {
		/* The creator of the user namespace has all caps. */
		if (targ_ns != &init_user_ns && targ_ns->creator == cred->user)
			return 0;

		/* Do we have the necessary capabilities? */
		if (targ_ns == cred->user->user_ns)
			return cap_raised(cred->cap_effective, cap) ? 0 : -EPERM;

		/* Have we tried all of the parent namespaces? */
		if (targ_ns == &init_user_ns)
			return -EPERM;

		/*
		 *If you have a capability in a parent user ns, then you have
		 * it over all children user namespaces as well.
		 */
		targ_ns = targ_ns->creator->user_ns;
	}

	/* We never get here */
}
