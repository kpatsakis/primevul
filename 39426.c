static void br_multicast_querier_expired(unsigned long data)
{
	struct net_bridge *br = (void *)data;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || br->multicast_disabled)
		goto out;

	br_multicast_start_querier(br);

out:
	spin_unlock(&br->multicast_lock);
}
