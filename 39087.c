static int oz_set_active_pd(const u8 *addr)
{
	int rc = 0;
	struct oz_pd *pd;
	struct oz_pd *old_pd;

	pd = oz_pd_find(addr);
	if (pd) {
		spin_lock_bh(&g_cdev.lock);
		memcpy(g_cdev.active_addr, addr, ETH_ALEN);
		old_pd = g_cdev.active_pd;
		g_cdev.active_pd = pd;
		spin_unlock_bh(&g_cdev.lock);
		if (old_pd)
			oz_pd_put(old_pd);
	} else {
		if (is_zero_ether_addr(addr)) {
			spin_lock_bh(&g_cdev.lock);
			pd = g_cdev.active_pd;
			g_cdev.active_pd = NULL;
			memset(g_cdev.active_addr, 0,
				sizeof(g_cdev.active_addr));
			spin_unlock_bh(&g_cdev.lock);
			if (pd)
				oz_pd_put(pd);
		} else {
			rc = -1;
		}
	}
	return rc;
}
