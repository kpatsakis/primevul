void kernel_power_off(void)
{
	kernel_shutdown_prepare(SYSTEM_POWER_OFF);
 if (pm_power_off_prepare)
		pm_power_off_prepare();
	migrate_to_reboot_cpu();
	syscore_shutdown();
	printk(KERN_EMERG "Power down.\n");
	kmsg_dump(KMSG_DUMP_POWEROFF);
	machine_power_off();
}
