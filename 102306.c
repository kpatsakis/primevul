SYSCALL_DEFINE3(setresuid, uid_t, ruid, uid_t, euid, uid_t, suid)
{
 const struct cred *old;
 struct cred *new;
 int retval;

 new = prepare_creds();
 if (!new)
 return -ENOMEM;

	old = current_cred();

	retval = -EPERM;
 if (!nsown_capable(CAP_SETUID)) {
 if (ruid != (uid_t) -1 && ruid != old->uid &&
		    ruid != old->euid  && ruid != old->suid)
 goto error;
 if (euid != (uid_t) -1 && euid != old->uid &&
		    euid != old->euid  && euid != old->suid)
 goto error;
 if (suid != (uid_t) -1 && suid != old->uid &&
		    suid != old->euid  && suid != old->suid)
 goto error;
 }

 if (ruid != (uid_t) -1) {
 new->uid = ruid;
 if (ruid != old->uid) {
			retval = set_user(new);
 if (retval < 0)
 goto error;
 }
 }
 if (euid != (uid_t) -1)
 new->euid = euid;
 if (suid != (uid_t) -1)
 new->suid = suid;
 new->fsuid = new->euid;

	retval = security_task_fix_setuid(new, old, LSM_SETID_RES);
 if (retval < 0)
 goto error;

 return commit_creds(new);

error:
	abort_creds(new);
 return retval;
}
