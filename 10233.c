static int rsi_load_9116_bootup_params(struct rsi_common *common)
{
	struct sk_buff *skb;
	struct rsi_boot_params_9116 *boot_params;

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending boot params frame\n", __func__);

	skb = dev_alloc_skb(sizeof(struct rsi_boot_params_9116));
	if (!skb)
		return -ENOMEM;
	memset(skb->data, 0, sizeof(struct rsi_boot_params));
	boot_params = (struct rsi_boot_params_9116 *)skb->data;

	if (common->channel_width == BW_40MHZ) {
		memcpy(&boot_params->bootup_params,
		       &boot_params_9116_40,
		       sizeof(struct bootup_params_9116));
		rsi_dbg(MGMT_TX_ZONE, "%s: Packet 40MHZ <=== %d\n", __func__,
			UMAC_CLK_40BW);
		boot_params->umac_clk = cpu_to_le16(UMAC_CLK_40BW);
	} else {
		memcpy(&boot_params->bootup_params,
		       &boot_params_9116_20,
		       sizeof(struct bootup_params_9116));
		if (boot_params_20.valid != cpu_to_le32(VALID_20)) {
			boot_params->umac_clk = cpu_to_le16(UMAC_CLK_20BW);
			rsi_dbg(MGMT_TX_ZONE,
				"%s: Packet 20MHZ <=== %d\n", __func__,
				UMAC_CLK_20BW);
		} else {
			boot_params->umac_clk = cpu_to_le16(UMAC_CLK_40MHZ);
			rsi_dbg(MGMT_TX_ZONE,
				"%s: Packet 20MHZ <=== %d\n", __func__,
				UMAC_CLK_40MHZ);
		}
	}
	rsi_set_len_qno(&boot_params->desc_dword0.len_qno,
			sizeof(struct bootup_params_9116), RSI_WIFI_MGMT_Q);
	boot_params->desc_dword0.frame_type = BOOTUP_PARAMS_REQUEST;
	skb_put(skb, sizeof(struct rsi_boot_params_9116));

	return rsi_send_internal_mgmt_frame(common, skb);
}