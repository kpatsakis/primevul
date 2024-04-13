void kernel_restart(char *cmd)
{
	kernel_restart_prepare(cmd);
	disable_nonboot_cpus();
	if (!cmd)
		printk(KERN_EMERG "Restarting system.\n");
	else
		printk(KERN_EMERG "Restarting system with command '%s'.\n", cmd);
	kmsg_dump(KMSG_DUMP_RESTART);
	machine_restart(cmd);
}
