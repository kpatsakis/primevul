static void print_track(const char *s, struct track *t)
{
	if (!t->addr)
		return;

	printk(KERN_ERR "INFO: %s in ", s);
	__print_symbol("%s", (unsigned long)t->addr);
	printk(" age=%lu cpu=%u pid=%d\n", jiffies - t->when, t->cpu, t->pid);
}
