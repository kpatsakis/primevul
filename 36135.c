SYSCALL_DEFINE0(getpgrp)
{
	return sys_getpgid(0);
}
