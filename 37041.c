static int __init mmtimer_init(void)
{
	cnodeid_t node, maxn = -1;

	if (!ia64_platform_is("sn2"))
		return 0;

	/*
	 * Sanity check the cycles/sec variable
	 */
	if (sn_rtc_cycles_per_second < 100000) {
		printk(KERN_ERR "%s: unable to determine clock frequency\n",
		       MMTIMER_NAME);
		goto out1;
	}

	mmtimer_femtoperiod = ((unsigned long)1E15 + sn_rtc_cycles_per_second /
			       2) / sn_rtc_cycles_per_second;

	if (request_irq(SGI_MMTIMER_VECTOR, mmtimer_interrupt, IRQF_PERCPU, MMTIMER_NAME, NULL)) {
		printk(KERN_WARNING "%s: unable to allocate interrupt.",
			MMTIMER_NAME);
		goto out1;
	}

	if (misc_register(&mmtimer_miscdev)) {
		printk(KERN_ERR "%s: failed to register device\n",
		       MMTIMER_NAME);
		goto out2;
	}

	/* Get max numbered node, calculate slots needed */
	for_each_online_node(node) {
		maxn = node;
	}
	maxn++;

	/* Allocate list of node ptrs to mmtimer_t's */
	timers = kzalloc(sizeof(struct mmtimer_node)*maxn, GFP_KERNEL);
	if (timers == NULL) {
		printk(KERN_ERR "%s: failed to allocate memory for device\n",
				MMTIMER_NAME);
		goto out3;
	}

	/* Initialize struct mmtimer's for each online node */
	for_each_online_node(node) {
		spin_lock_init(&timers[node].lock);
		tasklet_init(&timers[node].tasklet, mmtimer_tasklet,
			(unsigned long) node);
	}

	sgi_clock_period = sgi_clock.res = NSEC_PER_SEC / sn_rtc_cycles_per_second;
	register_posix_clock(CLOCK_SGI_CYCLE, &sgi_clock);

	printk(KERN_INFO "%s: v%s, %ld MHz\n", MMTIMER_DESC, MMTIMER_VERSION,
	       sn_rtc_cycles_per_second/(unsigned long)1E6);

	return 0;

out3:
	kfree(timers);
	misc_deregister(&mmtimer_miscdev);
out2:
	free_irq(SGI_MMTIMER_VECTOR, NULL);
out1:
	return -1;
}
