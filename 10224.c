int rsi_set_vap_capabilities(struct rsi_common *common,
			     enum opmode mode,
			     u8 *mac_addr,
			     u8 vap_id,
			     u8 vap_status)
{
	struct sk_buff *skb = NULL;
	struct rsi_vap_caps *vap_caps;
	struct rsi_hw *adapter = common->priv;
	struct ieee80211_hw *hw = adapter->hw;
	struct ieee80211_conf *conf = &hw->conf;
	u16 frame_len = sizeof(struct rsi_vap_caps);

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending VAP capabilities frame\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	vap_caps = (struct rsi_vap_caps *)skb->data;

	rsi_set_len_qno(&vap_caps->desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ), RSI_WIFI_MGMT_Q);
	vap_caps->desc_dword0.frame_type = VAP_CAPABILITIES;
	vap_caps->status = vap_status;
	vap_caps->vif_type = mode;
	vap_caps->channel_bw = common->channel_width;
	vap_caps->vap_id = vap_id;
	vap_caps->radioid_macid = ((common->mac_id & 0xf) << 4) |
				   (common->radio_id & 0xf);

	memcpy(vap_caps->mac_addr, mac_addr, IEEE80211_ADDR_LEN);
	vap_caps->keep_alive_period = cpu_to_le16(90);
	vap_caps->frag_threshold = cpu_to_le16(IEEE80211_MAX_FRAG_THRESHOLD);

	vap_caps->rts_threshold = cpu_to_le16(common->rts_threshold);

	if (common->band == NL80211_BAND_5GHZ) {
		vap_caps->default_ctrl_rate = cpu_to_le16(RSI_RATE_6);
		vap_caps->default_mgmt_rate = cpu_to_le32(RSI_RATE_6);
	} else {
		vap_caps->default_ctrl_rate = cpu_to_le16(RSI_RATE_1);
		vap_caps->default_mgmt_rate = cpu_to_le32(RSI_RATE_1);
	}
	if (conf_is_ht40(conf)) {
		if (conf_is_ht40_minus(conf))
			vap_caps->ctrl_rate_flags =
				cpu_to_le16(UPPER_20_ENABLE);
		else if (conf_is_ht40_plus(conf))
			vap_caps->ctrl_rate_flags =
				cpu_to_le16(LOWER_20_ENABLE);
		else
			vap_caps->ctrl_rate_flags =
				cpu_to_le16(FULL40M_ENABLE);
	}

	vap_caps->default_data_rate = 0;
	vap_caps->beacon_interval = cpu_to_le16(common->beacon_interval);
	vap_caps->dtim_period = cpu_to_le16(common->dtim_cnt);

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}