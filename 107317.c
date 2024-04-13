void drop_privs()
{
	if (chroot(".") == -1)
		err(1, "chroot()");

	if (setgroups(0, NULL) == -1)
		err(1, "setgroups()");

	if (setgid(69) == -1)
		err(1, "setgid()");

	if (setuid(69) == -1)
		err(1, "setuid()");
}
