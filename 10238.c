int rsi_handle_card_ready(struct rsi_common *common, u8 *msg)
{
	int status;

	switch (common->fsm_state) {
	case FSM_CARD_NOT_READY:
		rsi_dbg(INIT_ZONE, "Card ready indication from Common HAL\n");
		rsi_set_default_parameters(common);
		if (rsi_send_common_dev_params(common) < 0)
			return -EINVAL;
		common->fsm_state = FSM_COMMON_DEV_PARAMS_SENT;
		break;
	case FSM_COMMON_DEV_PARAMS_SENT:
		rsi_dbg(INIT_ZONE, "Card ready indication from WLAN HAL\n");

		if (common->priv->device_model == RSI_DEV_9116) {
			if (msg[16] != MAGIC_WORD) {
				rsi_dbg(FSM_ZONE,
					"%s: [EEPROM_READ] Invalid token\n",
					__func__);
				common->fsm_state = FSM_CARD_NOT_READY;
				return -EINVAL;
			}
			memcpy(common->mac_addr, &msg[20], ETH_ALEN);
			rsi_dbg(INIT_ZONE, "MAC Addr %pM", common->mac_addr);
		}
		/* Get usb buffer status register address */
		common->priv->usb_buffer_status_reg = *(u32 *)&msg[8];
		rsi_dbg(INFO_ZONE, "USB buffer status register = %x\n",
			common->priv->usb_buffer_status_reg);

		if (common->priv->device_model == RSI_DEV_9116)
			status = rsi_load_9116_bootup_params(common);
		else
			status = rsi_load_bootup_params(common);
		if (status < 0) {
			common->fsm_state = FSM_CARD_NOT_READY;
			return status;
		}
		common->fsm_state = FSM_BOOT_PARAMS_SENT;
		break;
	default:
		rsi_dbg(ERR_ZONE,
			"%s: card ready indication in invalid state %d.\n",
			__func__, common->fsm_state);
		return -EINVAL;
	}

	return 0;
}