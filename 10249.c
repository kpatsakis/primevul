static int rsi_load_radio_caps(struct rsi_common *common)
{
	struct rsi_radio_caps *radio_caps;
	struct rsi_hw *adapter = common->priv;
	u16 inx = 0;
	u8 ii;
	u8 radio_id = 0;
	u16 gc[20] = {0xf0, 0xf0, 0xf0, 0xf0,
		      0xf0, 0xf0, 0xf0, 0xf0,
		      0xf0, 0xf0, 0xf0, 0xf0,
		      0xf0, 0xf0, 0xf0, 0xf0,
		      0xf0, 0xf0, 0xf0, 0xf0};
	struct sk_buff *skb;
	u16 frame_len = sizeof(struct rsi_radio_caps);

	rsi_dbg(INFO_ZONE, "%s: Sending rate symbol req frame\n", __func__);

	skb = dev_alloc_skb(frame_len);

	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	radio_caps = (struct rsi_radio_caps *)skb->data;

	radio_caps->desc_dword0.frame_type = RADIO_CAPABILITIES;
	radio_caps->channel_num = common->channel;
	radio_caps->rf_model = RSI_RF_TYPE;

	radio_caps->radio_cfg_info = RSI_LMAC_CLOCK_80MHZ;
	if (common->channel_width == BW_40MHZ) {
		radio_caps->radio_cfg_info |= RSI_ENABLE_40MHZ;

		if (common->fsm_state == FSM_MAC_INIT_DONE) {
			struct ieee80211_hw *hw = adapter->hw;
			struct ieee80211_conf *conf = &hw->conf;

			if (conf_is_ht40_plus(conf)) {
				radio_caps->ppe_ack_rate =
					cpu_to_le16(LOWER_20_ENABLE |
						    (LOWER_20_ENABLE >> 12));
			} else if (conf_is_ht40_minus(conf)) {
				radio_caps->ppe_ack_rate =
					cpu_to_le16(UPPER_20_ENABLE |
						    (UPPER_20_ENABLE >> 12));
			} else {
				radio_caps->ppe_ack_rate =
					cpu_to_le16((BW_40MHZ << 12) |
						    FULL40M_ENABLE);
			}
		}
	}
	radio_caps->radio_info |= radio_id;

	if (adapter->device_model == RSI_DEV_9116 &&
	    common->channel_width == BW_20MHZ)
		radio_caps->radio_cfg_info &= ~0x3;

	radio_caps->sifs_tx_11n = cpu_to_le16(SIFS_TX_11N_VALUE);
	radio_caps->sifs_tx_11b = cpu_to_le16(SIFS_TX_11B_VALUE);
	radio_caps->slot_rx_11n = cpu_to_le16(SHORT_SLOT_VALUE);
	radio_caps->ofdm_ack_tout = cpu_to_le16(OFDM_ACK_TOUT_VALUE);
	radio_caps->cck_ack_tout = cpu_to_le16(CCK_ACK_TOUT_VALUE);
	radio_caps->preamble_type = cpu_to_le16(LONG_PREAMBLE);

	for (ii = 0; ii < MAX_HW_QUEUES; ii++) {
		radio_caps->qos_params[ii].cont_win_min_q = cpu_to_le16(3);
		radio_caps->qos_params[ii].cont_win_max_q = cpu_to_le16(0x3f);
		radio_caps->qos_params[ii].aifsn_val_q = cpu_to_le16(2);
		radio_caps->qos_params[ii].txop_q = 0;
	}

	for (ii = 0; ii < NUM_EDCA_QUEUES; ii++) {
		if (common->edca_params[ii].cw_max > 0) {
			radio_caps->qos_params[ii].cont_win_min_q =
				cpu_to_le16(common->edca_params[ii].cw_min);
			radio_caps->qos_params[ii].cont_win_max_q =
				cpu_to_le16(common->edca_params[ii].cw_max);
			radio_caps->qos_params[ii].aifsn_val_q =
				cpu_to_le16(common->edca_params[ii].aifs << 8);
			radio_caps->qos_params[ii].txop_q =
				cpu_to_le16(common->edca_params[ii].txop);
		}
	}

	radio_caps->qos_params[BROADCAST_HW_Q].txop_q = cpu_to_le16(0xffff);
	radio_caps->qos_params[MGMT_HW_Q].txop_q = 0;
	radio_caps->qos_params[BEACON_HW_Q].txop_q = cpu_to_le16(0xffff);

	memcpy(&common->rate_pwr[0], &gc[0], 40);
	for (ii = 0; ii < 20; ii++)
		radio_caps->gcpd_per_rate[inx++] =
			cpu_to_le16(common->rate_pwr[ii]  & 0x00FF);

	rsi_set_len_qno(&radio_caps->desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ), RSI_WIFI_MGMT_Q);

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}