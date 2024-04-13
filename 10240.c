int rsi_hal_send_sta_notify_frame(struct rsi_common *common, enum opmode opmode,
				  u8 notify_event, const unsigned char *bssid,
				  u8 qos_enable, u16 aid, u16 sta_id,
				  struct ieee80211_vif *vif)
{
	struct sk_buff *skb = NULL;
	struct rsi_peer_notify *peer_notify;
	u16 vap_id = ((struct vif_priv *)vif->drv_priv)->vap_id;
	int status;
	u16 frame_len = sizeof(struct rsi_peer_notify);

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending sta notify frame\n", __func__);

	skb = dev_alloc_skb(frame_len);

	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	peer_notify = (struct rsi_peer_notify *)skb->data;

	if (opmode == RSI_OPMODE_STA)
		peer_notify->command = cpu_to_le16(PEER_TYPE_AP << 1);
	else if (opmode == RSI_OPMODE_AP)
		peer_notify->command = cpu_to_le16(PEER_TYPE_STA << 1);

	switch (notify_event) {
	case STA_CONNECTED:
		peer_notify->command |= cpu_to_le16(RSI_ADD_PEER);
		break;
	case STA_DISCONNECTED:
		peer_notify->command |= cpu_to_le16(RSI_DELETE_PEER);
		break;
	default:
		break;
	}

	peer_notify->command |= cpu_to_le16((aid & 0xfff) << 4);
	ether_addr_copy(peer_notify->mac_addr, bssid);
	peer_notify->mpdu_density = cpu_to_le16(RSI_MPDU_DENSITY);
	peer_notify->sta_flags = cpu_to_le32((qos_enable) ? 1 : 0);

	rsi_set_len_qno(&peer_notify->desc.desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ),
			RSI_WIFI_MGMT_Q);
	peer_notify->desc.desc_dword0.frame_type = PEER_NOTIFY;
	peer_notify->desc.desc_dword3.qid_tid = sta_id;
	peer_notify->desc.desc_dword3.sta_id = vap_id;

	skb_put(skb, frame_len);

	status = rsi_send_internal_mgmt_frame(common, skb);

	if ((vif->type == NL80211_IFTYPE_STATION) &&
	    (!status && qos_enable)) {
		rsi_set_contention_vals(common);
		status = rsi_load_radio_caps(common);
	}
	return status;
}