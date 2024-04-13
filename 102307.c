SYSCALL_DEFINE3(getresuid, uid_t __user *, ruid, uid_t __user *, euid, uid_t __user *, suid)
{
 const struct cred *cred = current_cred();
 int retval;

 if (!(retval   = put_user(cred->uid,  ruid)) &&
 !(retval   = put_user(cred->euid, euid)))
		retval = put_user(cred->suid, suid);

 return retval;
}
