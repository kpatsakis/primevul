static int rsi_send_w9116_features(struct rsi_common *common)
{
	struct rsi_wlan_9116_features *w9116_features;
	u16 frame_len = sizeof(struct rsi_wlan_9116_features);
	struct sk_buff *skb;

	rsi_dbg(MGMT_TX_ZONE,
		"%s: Sending wlan 9116 features\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb)
		return -ENOMEM;
	memset(skb->data, 0, frame_len);

	w9116_features = (struct rsi_wlan_9116_features *)skb->data;

	w9116_features->pll_mode = common->w9116_features.pll_mode;
	w9116_features->rf_type = common->w9116_features.rf_type;
	w9116_features->wireless_mode = common->w9116_features.wireless_mode;
	w9116_features->enable_ppe = common->w9116_features.enable_ppe;
	w9116_features->afe_type = common->w9116_features.afe_type;
	if (common->w9116_features.dpd)
		w9116_features->feature_enable |= cpu_to_le32(RSI_DPD);
	if (common->w9116_features.sifs_tx_enable)
		w9116_features->feature_enable |=
			cpu_to_le32(RSI_SIFS_TX_ENABLE);
	if (common->w9116_features.ps_options & RSI_DUTY_CYCLING)
		w9116_features->feature_enable |= cpu_to_le32(RSI_DUTY_CYCLING);
	if (common->w9116_features.ps_options & RSI_END_OF_FRAME)
		w9116_features->feature_enable |= cpu_to_le32(RSI_END_OF_FRAME);
	w9116_features->feature_enable |=
		cpu_to_le32((common->w9116_features.ps_options & ~0x3) << 2);

	rsi_set_len_qno(&w9116_features->desc.desc_dword0.len_qno,
			frame_len - FRAME_DESC_SZ, RSI_WIFI_MGMT_Q);
	w9116_features->desc.desc_dword0.frame_type = FEATURES_ENABLE;
	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}