SYSCALL_DEFINE3(setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid)
{
	struct user_namespace *ns = current_user_ns();
	const struct cred *old;
	struct cred *new;
	int retval;
	kgid_t krgid, kegid, ksgid;

	krgid = make_kgid(ns, rgid);
	kegid = make_kgid(ns, egid);
	ksgid = make_kgid(ns, sgid);

	if ((rgid != (gid_t) -1) && !gid_valid(krgid))
		return -EINVAL;
	if ((egid != (gid_t) -1) && !gid_valid(kegid))
		return -EINVAL;
	if ((sgid != (gid_t) -1) && !gid_valid(ksgid))
		return -EINVAL;

	new = prepare_creds();
	if (!new)
		return -ENOMEM;
	old = current_cred();

	retval = -EPERM;
	if (!nsown_capable(CAP_SETGID)) {
		if (rgid != (gid_t) -1        && !gid_eq(krgid, old->gid) &&
		    !gid_eq(krgid, old->egid) && !gid_eq(krgid, old->sgid))
			goto error;
		if (egid != (gid_t) -1        && !gid_eq(kegid, old->gid) &&
		    !gid_eq(kegid, old->egid) && !gid_eq(kegid, old->sgid))
			goto error;
		if (sgid != (gid_t) -1        && !gid_eq(ksgid, old->gid) &&
		    !gid_eq(ksgid, old->egid) && !gid_eq(ksgid, old->sgid))
			goto error;
	}

	if (rgid != (gid_t) -1)
		new->gid = krgid;
	if (egid != (gid_t) -1)
		new->egid = kegid;
	if (sgid != (gid_t) -1)
		new->sgid = ksgid;
	new->fsgid = new->egid;

	return commit_creds(new);

error:
	abort_creds(new);
	return retval;
}
