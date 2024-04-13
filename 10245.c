int rsi_set_channel(struct rsi_common *common,
		    struct ieee80211_channel *channel)
{
	struct sk_buff *skb = NULL;
	struct rsi_chan_config *chan_cfg;
	u16 frame_len = sizeof(struct rsi_chan_config);

	rsi_dbg(MGMT_TX_ZONE,
		"%s: Sending scan req frame\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	if (!channel) {
		dev_kfree_skb(skb);
		return 0;
	}
	memset(skb->data, 0, frame_len);
	chan_cfg = (struct rsi_chan_config *)skb->data;

	rsi_set_len_qno(&chan_cfg->desc_dword0.len_qno, 0, RSI_WIFI_MGMT_Q);
	chan_cfg->desc_dword0.frame_type = SCAN_REQUEST;
	chan_cfg->channel_number = channel->hw_value;
	chan_cfg->antenna_gain_offset_2g = channel->max_antenna_gain;
	chan_cfg->antenna_gain_offset_5g = channel->max_antenna_gain;
	chan_cfg->region_rftype = (RSI_RF_TYPE & 0xf) << 4;

	if ((channel->flags & IEEE80211_CHAN_NO_IR) ||
	    (channel->flags & IEEE80211_CHAN_RADAR)) {
		chan_cfg->antenna_gain_offset_2g |= RSI_CHAN_RADAR;
	} else {
		if (common->tx_power < channel->max_power)
			chan_cfg->tx_power = cpu_to_le16(common->tx_power);
		else
			chan_cfg->tx_power = cpu_to_le16(channel->max_power);
	}
	chan_cfg->region_rftype |= (common->priv->dfs_region & 0xf);

	if (common->channel_width == BW_40MHZ)
		chan_cfg->channel_width = 0x1;

	common->channel = channel->hw_value;

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}