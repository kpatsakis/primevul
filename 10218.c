static int rsi_mgmt_pkt_to_core(struct rsi_common *common,
				u8 *msg,
				s32 msg_len)
{
	struct rsi_hw *adapter = common->priv;
	struct ieee80211_tx_info *info;
	struct skb_info *rx_params;
	u8 pad_bytes = msg[4];
	struct sk_buff *skb;

	if (!adapter->sc_nvifs)
		return -ENOLINK;

	msg_len -= pad_bytes;
	if (msg_len <= 0) {
		rsi_dbg(MGMT_RX_ZONE,
			"%s: Invalid rx msg of len = %d\n",
			__func__, msg_len);
		return -EINVAL;
	}

	skb = dev_alloc_skb(msg_len);
	if (!skb)
		return -ENOMEM;

	skb_put_data(skb,
		     (u8 *)(msg + FRAME_DESC_SZ + pad_bytes),
		     msg_len);

	info = IEEE80211_SKB_CB(skb);
	rx_params = (struct skb_info *)info->driver_data;
	rx_params->rssi = rsi_get_rssi(msg);
	rx_params->channel = rsi_get_channel(msg);
	rsi_indicate_pkt_to_os(common, skb);

	return 0;
}