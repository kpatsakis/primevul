static int rsi_send_common_dev_params(struct rsi_common *common)
{
	struct sk_buff *skb;
	u16 frame_len;
	struct rsi_config_vals *dev_cfgs;

	frame_len = sizeof(struct rsi_config_vals);

	rsi_dbg(MGMT_TX_ZONE, "Sending common device config params\n");
	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Unable to allocate skb\n", __func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);

	dev_cfgs = (struct rsi_config_vals *)skb->data;
	memset(dev_cfgs, 0, (sizeof(struct rsi_config_vals)));

	rsi_set_len_qno(&dev_cfgs->len_qno, (frame_len - FRAME_DESC_SZ),
			RSI_COEX_Q);
	dev_cfgs->pkt_type = COMMON_DEV_CONFIG;

	dev_cfgs->lp_ps_handshake = common->lp_ps_handshake_mode;
	dev_cfgs->ulp_ps_handshake = common->ulp_ps_handshake_mode;

	dev_cfgs->unused_ulp_gpio = RSI_UNUSED_ULP_GPIO_BITMAP;
	dev_cfgs->unused_soc_gpio_bitmap =
				cpu_to_le32(RSI_UNUSED_SOC_GPIO_BITMAP);

	dev_cfgs->opermode = common->oper_mode;
	dev_cfgs->wlan_rf_pwr_mode = common->wlan_rf_power_mode;
	dev_cfgs->driver_mode = common->driver_mode;
	dev_cfgs->region_code = NL80211_DFS_FCC;
	dev_cfgs->antenna_sel_val = common->obm_ant_sel_val;

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}