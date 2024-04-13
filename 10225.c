int rsi_hal_load_key(struct rsi_common *common,
		     u8 *data,
		     u16 key_len,
		     u8 key_type,
		     u8 key_id,
		     u32 cipher,
		     s16 sta_id,
		     struct ieee80211_vif *vif)
{
	struct sk_buff *skb = NULL;
	struct rsi_set_key *set_key;
	u16 key_descriptor = 0;
	u16 frame_len = sizeof(struct rsi_set_key);

	rsi_dbg(MGMT_TX_ZONE, "%s: Sending load key frame\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	set_key = (struct rsi_set_key *)skb->data;

	if (key_type == RSI_GROUP_KEY) {
		key_descriptor = RSI_KEY_TYPE_BROADCAST;
		if (vif->type == NL80211_IFTYPE_AP)
			key_descriptor |= RSI_KEY_MODE_AP;
	}
	if ((cipher == WLAN_CIPHER_SUITE_WEP40) ||
	    (cipher == WLAN_CIPHER_SUITE_WEP104)) {
		key_id = 0;
		key_descriptor |= RSI_WEP_KEY;
		if (key_len >= 13)
			key_descriptor |= RSI_WEP_KEY_104;
	} else if (cipher != KEY_TYPE_CLEAR) {
		key_descriptor |= RSI_CIPHER_WPA;
		if (cipher == WLAN_CIPHER_SUITE_TKIP)
			key_descriptor |= RSI_CIPHER_TKIP;
	}
	key_descriptor |= RSI_PROTECT_DATA_FRAMES;
	key_descriptor |= (key_id << RSI_KEY_ID_OFFSET);

	rsi_set_len_qno(&set_key->desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ), RSI_WIFI_MGMT_Q);
	set_key->desc_dword0.frame_type = SET_KEY_REQ;
	set_key->key_desc = cpu_to_le16(key_descriptor);
	set_key->sta_id = sta_id;

	if (data) {
		if ((cipher == WLAN_CIPHER_SUITE_WEP40) ||
		    (cipher == WLAN_CIPHER_SUITE_WEP104)) {
			memcpy(&set_key->key[key_id][1], data, key_len * 2);
		} else {
			memcpy(&set_key->key[0][0], data, key_len);
		}
		memcpy(set_key->tx_mic_key, &data[16], 8);
		memcpy(set_key->rx_mic_key, &data[24], 8);
	} else {
		memset(&set_key[FRAME_DESC_SZ], 0, frame_len - FRAME_DESC_SZ);
	}

	skb_put(skb, frame_len);

	return rsi_send_internal_mgmt_frame(common, skb);
}