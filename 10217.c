static int rsi_handle_ta_confirm_type(struct rsi_common *common,
				      u8 *msg)
{
	struct rsi_hw *adapter = common->priv;
	u8 sub_type = (msg[15] & 0xff);
	u16 msg_len = ((u16 *)msg)[0] & 0xfff;
	u8 offset;

	switch (sub_type) {
	case BOOTUP_PARAMS_REQUEST:
		rsi_dbg(FSM_ZONE, "%s: Boot up params confirm received\n",
			__func__);
		if (common->fsm_state == FSM_BOOT_PARAMS_SENT) {
			if (adapter->device_model == RSI_DEV_9116) {
				common->band = NL80211_BAND_5GHZ;
				common->num_supp_bands = 2;

				if (rsi_send_reset_mac(common))
					goto out;
				else
					common->fsm_state = FSM_RESET_MAC_SENT;
			} else {
				adapter->eeprom.length =
					(IEEE80211_ADDR_LEN +
					 WLAN_MAC_MAGIC_WORD_LEN +
					 WLAN_HOST_MODE_LEN);
				adapter->eeprom.offset = WLAN_MAC_EEPROM_ADDR;
				if (rsi_eeprom_read(common)) {
					common->fsm_state = FSM_CARD_NOT_READY;
					goto out;
				}
				common->fsm_state = FSM_EEPROM_READ_MAC_ADDR;
			}
		} else {
			rsi_dbg(INFO_ZONE,
				"%s: Received bootup params cfm in %d state\n",
				 __func__, common->fsm_state);
			return 0;
		}
		break;

	case EEPROM_READ:
		rsi_dbg(FSM_ZONE, "EEPROM READ confirm received\n");
		if (msg_len <= 0) {
			rsi_dbg(FSM_ZONE,
				"%s: [EEPROM_READ] Invalid len %d\n",
				__func__, msg_len);
			goto out;
		}
		if (msg[16] != MAGIC_WORD) {
			rsi_dbg(FSM_ZONE,
				"%s: [EEPROM_READ] Invalid token\n", __func__);
			common->fsm_state = FSM_CARD_NOT_READY;
			goto out;
		}
		if (common->fsm_state == FSM_EEPROM_READ_MAC_ADDR) {
			offset = (FRAME_DESC_SZ + WLAN_HOST_MODE_LEN +
				  WLAN_MAC_MAGIC_WORD_LEN);
			memcpy(common->mac_addr, &msg[offset], ETH_ALEN);
			adapter->eeprom.length =
				((WLAN_MAC_MAGIC_WORD_LEN + 3) & (~3));
			adapter->eeprom.offset = WLAN_EEPROM_RFTYPE_ADDR;
			if (rsi_eeprom_read(common)) {
				rsi_dbg(ERR_ZONE,
					"%s: Failed reading RF band\n",
					__func__);
				common->fsm_state = FSM_CARD_NOT_READY;
				goto out;
			}
			common->fsm_state = FSM_EEPROM_READ_RF_TYPE;
		} else if (common->fsm_state == FSM_EEPROM_READ_RF_TYPE) {
			if ((msg[17] & 0x3) == 0x3) {
				rsi_dbg(INIT_ZONE, "Dual band supported\n");
				common->band = NL80211_BAND_5GHZ;
				common->num_supp_bands = 2;
			} else if ((msg[17] & 0x3) == 0x1) {
				rsi_dbg(INIT_ZONE,
					"Only 2.4Ghz band supported\n");
				common->band = NL80211_BAND_2GHZ;
				common->num_supp_bands = 1;
			}
			if (rsi_send_reset_mac(common))
				goto out;
			common->fsm_state = FSM_RESET_MAC_SENT;
		} else {
			rsi_dbg(ERR_ZONE, "%s: Invalid EEPROM read type\n",
				__func__);
			return 0;
		}
		break;

	case RESET_MAC_REQ:
		if (common->fsm_state == FSM_RESET_MAC_SENT) {
			rsi_dbg(FSM_ZONE, "%s: Reset MAC cfm received\n",
				__func__);

			if (rsi_load_radio_caps(common))
				goto out;
			else
				common->fsm_state = FSM_RADIO_CAPS_SENT;
		} else {
			rsi_dbg(ERR_ZONE,
				"%s: Received reset mac cfm in %d state\n",
				 __func__, common->fsm_state);
			return 0;
		}
		break;

	case RADIO_CAPABILITIES:
		if (common->fsm_state == FSM_RADIO_CAPS_SENT) {
			common->rf_reset = 1;
			if (adapter->device_model == RSI_DEV_9116 &&
			    rsi_send_w9116_features(common)) {
				rsi_dbg(ERR_ZONE,
					"Failed to send 9116 features\n");
				goto out;
			}
			if (rsi_program_bb_rf(common)) {
				goto out;
			} else {
				common->fsm_state = FSM_BB_RF_PROG_SENT;
				rsi_dbg(FSM_ZONE, "%s: Radio cap cfm received\n",
					__func__);
			}
		} else {
			rsi_dbg(INFO_ZONE,
				"%s: Received radio caps cfm in %d state\n",
				 __func__, common->fsm_state);
			return 0;
		}
		break;

	case BB_PROG_VALUES_REQUEST:
	case RF_PROG_VALUES_REQUEST:
	case BBP_PROG_IN_TA:
		rsi_dbg(FSM_ZONE, "%s: BB/RF cfm received\n", __func__);
		if (common->fsm_state == FSM_BB_RF_PROG_SENT) {
			common->bb_rf_prog_count--;
			if (!common->bb_rf_prog_count) {
				common->fsm_state = FSM_MAC_INIT_DONE;
				if (common->reinit_hw) {
					complete(&common->wlan_init_completion);
				} else {
					return rsi_mac80211_attach(common);
				}
			}
		} else {
			rsi_dbg(INFO_ZONE,
				"%s: Received bbb_rf cfm in %d state\n",
				 __func__, common->fsm_state);
			return 0;
		}
		break;

	case SCAN_REQUEST:
		rsi_dbg(INFO_ZONE, "Set channel confirm\n");
		break;

	case WAKEUP_SLEEP_REQUEST:
		rsi_dbg(INFO_ZONE, "Wakeup/Sleep confirmation.\n");
		return rsi_handle_ps_confirm(adapter, msg);

	case BG_SCAN_PROBE_REQ:
		rsi_dbg(INFO_ZONE, "BG scan complete event\n");
		if (common->bgscan_en) {
			struct cfg80211_scan_info info;

			if (!rsi_send_bgscan_params(common, RSI_STOP_BGSCAN))
				common->bgscan_en = 0;
			info.aborted = false;
			ieee80211_scan_completed(adapter->hw, &info);
		}
		rsi_dbg(INFO_ZONE, "Background scan completed\n");
		break;

	default:
		rsi_dbg(INFO_ZONE, "%s: Invalid TA confirm pkt received\n",
			__func__);
		break;
	}
	return 0;
out:
	rsi_dbg(ERR_ZONE, "%s: Unable to send pkt/Invalid frame received\n",
		__func__);
	return -EINVAL;
}