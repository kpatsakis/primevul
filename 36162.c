static int __orderly_poweroff(void)
{
	int argc;
	char **argv;
	static char *envp[] = {
		"HOME=/",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin",
		NULL
	};
	int ret;

	argv = argv_split(GFP_ATOMIC, poweroff_cmd, &argc);
	if (argv == NULL) {
		printk(KERN_WARNING "%s failed to allocate memory for \"%s\"\n",
		       __func__, poweroff_cmd);
		return -ENOMEM;
	}

	ret = call_usermodehelper_fns(argv[0], argv, envp, UMH_WAIT_EXEC,
				      NULL, argv_cleanup, NULL);
	if (ret == -ENOMEM)
		argv_free(argv);

	return ret;
}
