int rsi_send_ps_request(struct rsi_hw *adapter, bool enable,
			struct ieee80211_vif *vif)
{
	struct rsi_common *common = adapter->priv;
	struct ieee80211_bss_conf *bss = &vif->bss_conf;
	struct rsi_request_ps *ps;
	struct rsi_ps_info *ps_info;
	struct sk_buff *skb;
	int frame_len = sizeof(*ps);

	skb = dev_alloc_skb(frame_len);
	if (!skb)
		return -ENOMEM;
	memset(skb->data, 0, frame_len);

	ps = (struct rsi_request_ps *)skb->data;
	ps_info = &adapter->ps_info;

	rsi_set_len_qno(&ps->desc.desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ), RSI_WIFI_MGMT_Q);
	ps->desc.desc_dword0.frame_type = WAKEUP_SLEEP_REQUEST;
	if (enable) {
		ps->ps_sleep.enable = RSI_PS_ENABLE;
		ps->desc.desc_dword3.token = cpu_to_le16(RSI_SLEEP_REQUEST);
	} else {
		ps->ps_sleep.enable = RSI_PS_DISABLE;
		ps->desc.desc_dword0.len_qno |= cpu_to_le16(RSI_PS_DISABLE_IND);
		ps->desc.desc_dword3.token = cpu_to_le16(RSI_WAKEUP_REQUEST);
	}

	ps->ps_uapsd_acs = common->uapsd_bitmap;

	ps->ps_sleep.sleep_type = ps_info->sleep_type;
	ps->ps_sleep.num_bcns_per_lis_int =
		cpu_to_le16(ps_info->num_bcns_per_lis_int);
	ps->ps_sleep.sleep_duration =
		cpu_to_le32(ps_info->deep_sleep_wakeup_period);

	if (bss->assoc)
		ps->ps_sleep.connected_sleep = RSI_CONNECTED_SLEEP;
	else
		ps->ps_sleep.connected_sleep = RSI_DEEP_SLEEP;

	ps->ps_listen_interval = cpu_to_le32(ps_info->listen_interval);
	ps->ps_dtim_interval_duration =
		cpu_to_le32(ps_info->dtim_interval_duration);

	if (ps_info->listen_interval > ps_info->dtim_interval_duration)
		ps->ps_listen_interval = cpu_to_le32(RSI_PS_DISABLE);

	ps->ps_num_dtim_intervals = cpu_to_le16(ps_info->num_dtims_per_sleep);
	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}