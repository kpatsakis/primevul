int rsi_send_vap_dynamic_update(struct rsi_common *common)
{
	struct sk_buff *skb;
	struct rsi_dynamic_s *dynamic_frame;

	rsi_dbg(MGMT_TX_ZONE,
		"%s: Sending vap update indication frame\n", __func__);

	skb = dev_alloc_skb(sizeof(struct rsi_dynamic_s));
	if (!skb)
		return -ENOMEM;

	memset(skb->data, 0, sizeof(struct rsi_dynamic_s));
	dynamic_frame = (struct rsi_dynamic_s *)skb->data;
	rsi_set_len_qno(&dynamic_frame->desc_dword0.len_qno,
			sizeof(dynamic_frame->frame_body), RSI_WIFI_MGMT_Q);

	dynamic_frame->desc_dword0.frame_type = VAP_DYNAMIC_UPDATE;
	dynamic_frame->desc_dword2.pkt_info =
					cpu_to_le32(common->rts_threshold);

	if (common->wow_flags & RSI_WOW_ENABLED) {
		/* Beacon miss threshold */
		dynamic_frame->desc_dword3.token =
					cpu_to_le16(RSI_BCN_MISS_THRESHOLD);
		dynamic_frame->frame_body.keep_alive_period =
					cpu_to_le16(RSI_WOW_KEEPALIVE);
	} else {
		dynamic_frame->frame_body.keep_alive_period =
					cpu_to_le16(RSI_DEF_KEEPALIVE);
	}

	dynamic_frame->desc_dword3.sta_id = 0; /* vap id */

	skb_put(skb, sizeof(struct rsi_dynamic_s));

	return rsi_send_internal_mgmt_frame(common, skb);
}