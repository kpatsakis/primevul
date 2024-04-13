int rsi_send_bgscan_params(struct rsi_common *common, int enable)
{
	struct rsi_bgscan_params *params = &common->bgscan;
	struct cfg80211_scan_request *scan_req = common->hwscan;
	struct rsi_bgscan_config *bgscan;
	struct sk_buff *skb;
	u16 frame_len = sizeof(*bgscan);
	u8 i;

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending bgscan params frame\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb)
		return -ENOMEM;
	memset(skb->data, 0, frame_len);

	bgscan = (struct rsi_bgscan_config *)skb->data;
	rsi_set_len_qno(&bgscan->desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ), RSI_WIFI_MGMT_Q);
	bgscan->desc_dword0.frame_type = BG_SCAN_PARAMS;
	bgscan->bgscan_threshold = cpu_to_le16(params->bgscan_threshold);
	bgscan->roam_threshold = cpu_to_le16(params->roam_threshold);
	if (enable)
		bgscan->bgscan_periodicity =
			cpu_to_le16(params->bgscan_periodicity);
	bgscan->active_scan_duration =
			cpu_to_le16(params->active_scan_duration);
	bgscan->passive_scan_duration =
			cpu_to_le16(params->passive_scan_duration);
	bgscan->two_probe = params->two_probe;

	bgscan->num_bgscan_channels = scan_req->n_channels;
	for (i = 0; i < bgscan->num_bgscan_channels; i++)
		bgscan->channels2scan[i] =
			cpu_to_le16(scan_req->channels[i]->hw_value);

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}