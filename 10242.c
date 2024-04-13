int rsi_set_antenna(struct rsi_common *common, u8 antenna)
{
	struct rsi_ant_sel_frame *ant_sel_frame;
	struct sk_buff *skb;

	skb = dev_alloc_skb(FRAME_DESC_SZ);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, FRAME_DESC_SZ);

	ant_sel_frame = (struct rsi_ant_sel_frame *)skb->data;
	ant_sel_frame->desc_dword0.frame_type = ANT_SEL_FRAME;
	ant_sel_frame->sub_frame_type = ANTENNA_SEL_TYPE;
	ant_sel_frame->ant_value = cpu_to_le16(antenna & ANTENNA_MASK_VALUE);
	rsi_set_len_qno(&ant_sel_frame->desc_dword0.len_qno,
			0, RSI_WIFI_MGMT_Q);
	skb_put(skb, FRAME_DESC_SZ);

	return rsi_send_internal_mgmt_frame(common, skb);
}