static int orinoco_ioctl_setfreq(struct net_device *dev,
				 struct iw_request_info *info,
				 struct iw_freq *frq,
				 char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int chan = -1;
	unsigned long flags;
	int err = -EINPROGRESS;		/* Call commit handler */

	/* In infrastructure mode the AP sets the channel */
	if (priv->iw_mode == NL80211_IFTYPE_STATION)
		return -EBUSY;

	if ((frq->e == 0) && (frq->m <= 1000)) {
		/* Setting by channel number */
		chan = frq->m;
	} else {
		/* Setting by frequency */
		int denom = 1;
		int i;

		/* Calculate denominator to rescale to MHz */
		for (i = 0; i < (6 - frq->e); i++)
			denom *= 10;

		chan = ieee80211_freq_to_dsss_chan(frq->m / denom);
	}

	if ((chan < 1) || (chan > NUM_CHANNELS) ||
	     !(priv->channel_mask & (1 << (chan-1))))
		return -EINVAL;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	priv->channel = chan;
	if (priv->iw_mode == NL80211_IFTYPE_MONITOR) {
		/* Fast channel change - no commit if successful */
		hermes_t *hw = &priv->hw;
		err = hw->ops->cmd_wait(hw, HERMES_CMD_TEST |
					    HERMES_TEST_SET_CHANNEL,
					chan, NULL);
	}
	orinoco_unlock(priv, &flags);

	return err;
}
