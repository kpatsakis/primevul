int rsi_send_wowlan_request(struct rsi_common *common, u16 flags,
			    u16 sleep_status)
{
	struct rsi_wowlan_req *cmd_frame;
	struct sk_buff *skb;
	u8 length;

	rsi_dbg(ERR_ZONE, "%s: Sending wowlan request frame\n", __func__);

	length = sizeof(*cmd_frame);
	skb = dev_alloc_skb(length);
	if (!skb)
		return -ENOMEM;
	memset(skb->data, 0, length);
	cmd_frame = (struct rsi_wowlan_req *)skb->data;

	rsi_set_len_qno(&cmd_frame->desc.desc_dword0.len_qno,
			(length - FRAME_DESC_SZ),
			RSI_WIFI_MGMT_Q);
	cmd_frame->desc.desc_dword0.frame_type = WOWLAN_CONFIG_PARAMS;
	cmd_frame->host_sleep_status = sleep_status;
	if (common->secinfo.security_enable &&
	    common->secinfo.gtk_cipher)
		flags |= RSI_WOW_GTK_REKEY;
	if (sleep_status)
		cmd_frame->wow_flags = flags;
	rsi_dbg(INFO_ZONE, "Host_Sleep_Status : %d Flags : %d\n",
		cmd_frame->host_sleep_status, cmd_frame->wow_flags);

	skb_put(skb, length);

	return rsi_send_internal_mgmt_frame(common, skb);
}