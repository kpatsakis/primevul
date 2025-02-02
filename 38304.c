void fib6_run_gc(unsigned long expires, struct net *net, bool force)
{
	unsigned long now;

	if (force) {
		spin_lock_bh(&fib6_gc_lock);
	} else if (!spin_trylock_bh(&fib6_gc_lock)) {
		mod_timer(&net->ipv6.ip6_fib_timer, jiffies + HZ);
		return;
	}
	gc_args.timeout = expires ? (int)expires :
			  net->ipv6.sysctl.ip6_rt_gc_interval;

	gc_args.more = icmp6_dst_gc();

	fib6_clean_all(net, fib6_age, 0, NULL);
	now = jiffies;
	net->ipv6.ip6_rt_last_gc = now;

	if (gc_args.more)
		mod_timer(&net->ipv6.ip6_fib_timer,
			  round_jiffies(now
					+ net->ipv6.sysctl.ip6_rt_gc_interval));
	else
		del_timer(&net->ipv6.ip6_fib_timer);
	spin_unlock_bh(&fib6_gc_lock);
}
