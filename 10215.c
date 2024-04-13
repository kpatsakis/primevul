static int rsi_eeprom_read(struct rsi_common *common)
{
	struct rsi_eeprom_read_frame *mgmt_frame;
	struct rsi_hw *adapter = common->priv;
	struct sk_buff *skb;

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending EEPROM read req frame\n", __func__);

	skb = dev_alloc_skb(FRAME_DESC_SZ);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, FRAME_DESC_SZ);
	mgmt_frame = (struct rsi_eeprom_read_frame *)skb->data;

	/* FrameType */
	rsi_set_len_qno(&mgmt_frame->len_qno, 0, RSI_WIFI_MGMT_Q);
	mgmt_frame->pkt_type = EEPROM_READ;

	/* Number of bytes to read */
	mgmt_frame->pkt_info =
		cpu_to_le32((adapter->eeprom.length << RSI_EEPROM_LEN_OFFSET) &
			    RSI_EEPROM_LEN_MASK);
	mgmt_frame->pkt_info |= cpu_to_le32((3 << RSI_EEPROM_HDR_SIZE_OFFSET) &
					    RSI_EEPROM_HDR_SIZE_MASK);

	/* Address to read */
	mgmt_frame->eeprom_offset = cpu_to_le32(adapter->eeprom.offset);

	skb_put(skb, FRAME_DESC_SZ);

	return rsi_send_internal_mgmt_frame(common, skb);
}