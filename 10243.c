static int rsi_program_bb_rf(struct rsi_common *common)
{
	struct sk_buff *skb;
	struct rsi_bb_rf_prog *bb_rf_prog;
	u16 frame_len = sizeof(struct rsi_bb_rf_prog);

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending program BB/RF frame\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	bb_rf_prog = (struct rsi_bb_rf_prog *)skb->data;

	rsi_set_len_qno(&bb_rf_prog->desc_dword0.len_qno, 0, RSI_WIFI_MGMT_Q);
	bb_rf_prog->desc_dword0.frame_type = BBP_PROG_IN_TA;
	bb_rf_prog->endpoint = common->endpoint;
	bb_rf_prog->rf_power_mode = common->wlan_rf_power_mode;

	if (common->rf_reset) {
		bb_rf_prog->flags =  cpu_to_le16(RF_RESET_ENABLE);
		rsi_dbg(MGMT_TX_ZONE, "%s: ===> RF RESET REQUEST SENT <===\n",
			__func__);
		common->rf_reset = 0;
	}
	common->bb_rf_prog_count = 1;
	bb_rf_prog->flags |= cpu_to_le16(PUT_BBP_RESET | BBP_REG_WRITE |
					 (RSI_RF_TYPE << 4));
	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}