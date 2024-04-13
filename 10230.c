int rsi_send_block_unblock_frame(struct rsi_common *common, bool block_event)
{
	struct rsi_block_unblock_data *mgmt_frame;
	struct sk_buff *skb;

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending block/unblock frame\n", __func__);

	skb = dev_alloc_skb(FRAME_DESC_SZ);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, FRAME_DESC_SZ);
	mgmt_frame = (struct rsi_block_unblock_data *)skb->data;

	rsi_set_len_qno(&mgmt_frame->desc_dword0.len_qno, 0, RSI_WIFI_MGMT_Q);
	mgmt_frame->desc_dword0.frame_type = BLOCK_HW_QUEUE;
	mgmt_frame->host_quiet_info = QUIET_INFO_VALID;

	if (block_event) {
		rsi_dbg(INFO_ZONE, "blocking the data qs\n");
		mgmt_frame->block_q_bitmap = cpu_to_le16(0xf);
		mgmt_frame->block_q_bitmap |= cpu_to_le16(0xf << 4);
	} else {
		rsi_dbg(INFO_ZONE, "unblocking the data qs\n");
		mgmt_frame->unblock_q_bitmap = cpu_to_le16(0xf);
		mgmt_frame->unblock_q_bitmap |= cpu_to_le16(0xf << 4);
	}

	skb_put(skb, FRAME_DESC_SZ);

	return rsi_send_internal_mgmt_frame(common, skb);
}