static struct iw_statistics *orinoco_get_wireless_stats(struct net_device *dev)
{
	struct orinoco_private *priv = ndev_priv(dev);
	hermes_t *hw = &priv->hw;
	struct iw_statistics *wstats = &priv->wstats;
	int err;
	unsigned long flags;

	if (!netif_device_present(dev)) {
		printk(KERN_WARNING "%s: get_wireless_stats() called while device not present\n",
		       dev->name);
		return NULL; /* FIXME: Can we do better than this? */
	}

	/* If busy, return the old stats.  Returning NULL may cause
	 * the interface to disappear from /proc/net/wireless */
	if (orinoco_lock(priv, &flags) != 0)
		return wstats;

	/* We can't really wait for the tallies inquiry command to
	 * complete, so we just use the previous results and trigger
	 * a new tallies inquiry command for next time - Jean II */
	/* FIXME: Really we should wait for the inquiry to come back -
	 * as it is the stats we give don't make a whole lot of sense.
	 * Unfortunately, it's not clear how to do that within the
	 * wireless extensions framework: I think we're in user
	 * context, but a lock seems to be held by the time we get in
	 * here so we're not safe to sleep here. */
	hermes_inquire(hw, HERMES_INQ_TALLIES);

	if (priv->iw_mode == NL80211_IFTYPE_ADHOC) {
		memset(&wstats->qual, 0, sizeof(wstats->qual));
		/* If a spy address is defined, we report stats of the
		 * first spy address - Jean II */
		if (SPY_NUMBER(priv)) {
			wstats->qual.qual = priv->spy_data.spy_stat[0].qual;
			wstats->qual.level = priv->spy_data.spy_stat[0].level;
			wstats->qual.noise = priv->spy_data.spy_stat[0].noise;
			wstats->qual.updated =
				priv->spy_data.spy_stat[0].updated;
		}
	} else {
		struct {
			__le16 qual, signal, noise, unused;
		} __packed cq;

		err = HERMES_READ_RECORD(hw, USER_BAP,
					 HERMES_RID_COMMSQUALITY, &cq);

		if (!err) {
			wstats->qual.qual = (int)le16_to_cpu(cq.qual);
			wstats->qual.level = (int)le16_to_cpu(cq.signal) - 0x95;
			wstats->qual.noise = (int)le16_to_cpu(cq.noise) - 0x95;
			wstats->qual.updated =
				IW_QUAL_ALL_UPDATED | IW_QUAL_DBM;
		}
	}

	orinoco_unlock(priv, &flags);
	return wstats;
}
