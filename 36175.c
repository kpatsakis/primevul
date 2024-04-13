int orderly_poweroff(bool force)
{
	int ret = __orderly_poweroff();

	if (ret && force) {
		printk(KERN_WARNING "Failed to start orderly shutdown: "
		       "forcing the issue\n");

		/*
		 * I guess this should try to kick off some daemon to sync and
		 * poweroff asap.  Or not even bother syncing if we're doing an
		 * emergency shutdown?
		 */
		emergency_sync();
		kernel_power_off();
	}

	return ret;
}
