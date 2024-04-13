int rsi_send_bgscan_probe_req(struct rsi_common *common,
			      struct ieee80211_vif *vif)
{
	struct cfg80211_scan_request *scan_req = common->hwscan;
	struct rsi_bgscan_probe *bgscan;
	struct sk_buff *skb;
	struct sk_buff *probereq_skb;
	u16 frame_len = sizeof(*bgscan);
	size_t ssid_len = 0;
	u8 *ssid = NULL;

	rsi_dbg(MGMT_TX_ZONE,
		"%s: Sending bgscan probe req frame\n", __func__);

	if (common->priv->sc_nvifs <= 0)
		return -ENODEV;

	if (scan_req->n_ssids) {
		ssid = scan_req->ssids[0].ssid;
		ssid_len = scan_req->ssids[0].ssid_len;
	}

	skb = dev_alloc_skb(frame_len + MAX_BGSCAN_PROBE_REQ_LEN);
	if (!skb)
		return -ENOMEM;
	memset(skb->data, 0, frame_len + MAX_BGSCAN_PROBE_REQ_LEN);

	bgscan = (struct rsi_bgscan_probe *)skb->data;
	bgscan->desc_dword0.frame_type = BG_SCAN_PROBE_REQ;
	bgscan->flags = cpu_to_le16(HOST_BG_SCAN_TRIG);
	if (common->band == NL80211_BAND_5GHZ) {
		bgscan->mgmt_rate = cpu_to_le16(RSI_RATE_6);
		bgscan->def_chan = cpu_to_le16(40);
	} else {
		bgscan->mgmt_rate = cpu_to_le16(RSI_RATE_1);
		bgscan->def_chan = cpu_to_le16(11);
	}
	bgscan->channel_scan_time = cpu_to_le16(RSI_CHANNEL_SCAN_TIME);

	probereq_skb = ieee80211_probereq_get(common->priv->hw, vif->addr, ssid,
					      ssid_len, scan_req->ie_len);
	if (!probereq_skb) {
		dev_kfree_skb(skb);
		return -ENOMEM;
	}

	memcpy(&skb->data[frame_len], probereq_skb->data, probereq_skb->len);

	bgscan->probe_req_length = cpu_to_le16(probereq_skb->len);

	rsi_set_len_qno(&bgscan->desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ + probereq_skb->len),
			RSI_WIFI_MGMT_Q);

	skb_put(skb, frame_len + probereq_skb->len);

	dev_kfree_skb(probereq_skb);

	return rsi_send_internal_mgmt_frame(common, skb);
}