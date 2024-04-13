static int make_proc_exitcode(void)
{
	struct proc_dir_entry *ent;

	ent = proc_create("exitcode", 0600, NULL, &exitcode_proc_fops);
	if (ent == NULL) {
		printk(KERN_WARNING "make_proc_exitcode : Failed to register "
		       "/proc/exitcode\n");
		return 0;
	}
	return 0;
}
