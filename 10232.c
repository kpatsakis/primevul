void rsi_inform_bss_status(struct rsi_common *common,
			   enum opmode opmode,
			   u8 status,
			   const u8 *addr,
			   u8 qos_enable,
			   u16 aid,
			   struct ieee80211_sta *sta,
			   u16 sta_id,
			   u16 assoc_cap,
			   struct ieee80211_vif *vif)
{
	if (status) {
		if (opmode == RSI_OPMODE_STA)
			common->hw_data_qs_blocked = true;
		rsi_hal_send_sta_notify_frame(common,
					      opmode,
					      STA_CONNECTED,
					      addr,
					      qos_enable,
					      aid, sta_id,
					      vif);
		if (common->min_rate == 0xffff)
			rsi_send_auto_rate_request(common, sta, sta_id, vif);
		if (opmode == RSI_OPMODE_STA &&
		    !(assoc_cap & WLAN_CAPABILITY_PRIVACY) &&
		    !rsi_send_block_unblock_frame(common, false))
			common->hw_data_qs_blocked = false;
	} else {
		if (opmode == RSI_OPMODE_STA)
			common->hw_data_qs_blocked = true;

		if (!(common->wow_flags & RSI_WOW_ENABLED))
			rsi_hal_send_sta_notify_frame(common, opmode,
						      STA_DISCONNECTED, addr,
						      qos_enable, aid, sta_id,
						      vif);
		if (opmode == RSI_OPMODE_STA)
			rsi_send_block_unblock_frame(common, true);
	}
}