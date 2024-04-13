int rsi_send_aggregation_params_frame(struct rsi_common *common,
				      u16 tid,
				      u16 ssn,
				      u8 buf_size,
				      u8 event,
				      u8 sta_id)
{
	struct sk_buff *skb = NULL;
	struct rsi_aggr_params *aggr_params;
	u16 frame_len = sizeof(struct rsi_aggr_params);

	skb = dev_alloc_skb(frame_len);

	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	aggr_params = (struct rsi_aggr_params *)skb->data;

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending AMPDU indication frame\n", __func__);

	rsi_set_len_qno(&aggr_params->desc_dword0.len_qno, 0, RSI_WIFI_MGMT_Q);
	aggr_params->desc_dword0.frame_type = AMPDU_IND;

	aggr_params->aggr_params = tid & RSI_AGGR_PARAMS_TID_MASK;
	aggr_params->peer_id = sta_id;
	if (event == STA_TX_ADDBA_DONE) {
		aggr_params->seq_start = cpu_to_le16(ssn);
		aggr_params->baw_size = cpu_to_le16(buf_size);
		aggr_params->aggr_params |= RSI_AGGR_PARAMS_START;
	} else if (event == STA_RX_ADDBA_DONE) {
		aggr_params->seq_start = cpu_to_le16(ssn);
		aggr_params->aggr_params |= (RSI_AGGR_PARAMS_START |
					     RSI_AGGR_PARAMS_RX_AGGR);
	} else if (event == STA_RX_DELBA) {
		aggr_params->aggr_params |= RSI_AGGR_PARAMS_RX_AGGR;
	}

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}