int rsi_mgmt_pkt_recv(struct rsi_common *common, u8 *msg)
{
	s32 msg_len = (le16_to_cpu(*(__le16 *)&msg[0]) & 0x0fff);
	u16 msg_type = (msg[2]);

	rsi_dbg(FSM_ZONE, "%s: Msg Len: %d, Msg Type: %4x\n",
		__func__, msg_len, msg_type);

	switch (msg_type) {
	case TA_CONFIRM_TYPE:
		return rsi_handle_ta_confirm_type(common, msg);
	case CARD_READY_IND:
		common->hibernate_resume = false;
		rsi_dbg(FSM_ZONE, "%s: Card ready indication received\n",
			__func__);
		return rsi_handle_card_ready(common, msg);
	case TX_STATUS_IND:
		switch (msg[RSI_TX_STATUS_TYPE]) {
		case PROBEREQ_CONFIRM:
			common->mgmt_q_block = false;
			rsi_dbg(FSM_ZONE, "%s: Probe confirm received\n",
				__func__);
			break;
		case EAPOL4_CONFIRM:
			if (msg[RSI_TX_STATUS]) {
				common->eapol4_confirm = true;
				if (!rsi_send_block_unblock_frame(common,
								  false))
					common->hw_data_qs_blocked = false;
			}
		}
		break;
	case BEACON_EVENT_IND:
		rsi_dbg(INFO_ZONE, "Beacon event\n");
		if (common->fsm_state != FSM_MAC_INIT_DONE)
			return -1;
		if (common->iface_down)
			return -1;
		if (!common->beacon_enabled)
			return -1;
		rsi_send_beacon(common);
		break;
	case WOWLAN_WAKEUP_REASON:
		rsi_dbg(ERR_ZONE, "\n\nWakeup Type: %x\n", msg[15]);
		switch (msg[15]) {
		case RSI_UNICAST_MAGIC_PKT:
			rsi_dbg(ERR_ZONE,
				"*** Wakeup for Unicast magic packet ***\n");
			break;
		case RSI_BROADCAST_MAGICPKT:
			rsi_dbg(ERR_ZONE,
				"*** Wakeup for Broadcast magic packet ***\n");
			break;
		case RSI_EAPOL_PKT:
			rsi_dbg(ERR_ZONE,
				"*** Wakeup for GTK renewal ***\n");
			break;
		case RSI_DISCONNECT_PKT:
			rsi_dbg(ERR_ZONE,
				"*** Wakeup for Disconnect ***\n");
			break;
		case RSI_HW_BMISS_PKT:
			rsi_dbg(ERR_ZONE,
				"*** Wakeup for HW Beacon miss ***\n");
			break;
		default:
			rsi_dbg(ERR_ZONE,
				"##### Un-intentional Wakeup #####\n");
			break;
	}
	break;
	case RX_DOT11_MGMT:
		return rsi_mgmt_pkt_to_core(common, msg, msg_len);
	default:
		rsi_dbg(INFO_ZONE, "Received packet type: 0x%x\n", msg_type);
	}
	return 0;
}