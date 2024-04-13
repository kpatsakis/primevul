int br_multicast_set_querier(struct net_bridge *br, unsigned long val)
{
	val = !!val;

	spin_lock_bh(&br->multicast_lock);
	if (br->multicast_querier == val)
		goto unlock;

	br->multicast_querier = val;
	if (val)
		br_multicast_start_querier(br);

unlock:
	spin_unlock_bh(&br->multicast_lock);

	return 0;
}
