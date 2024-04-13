static int rsi_send_auto_rate_request(struct rsi_common *common,
				      struct ieee80211_sta *sta,
				      u16 sta_id,
				      struct ieee80211_vif *vif)
{
	struct sk_buff *skb;
	struct rsi_auto_rate *auto_rate;
	int ii = 0, jj = 0, kk = 0;
	struct ieee80211_hw *hw = common->priv->hw;
	u8 band = hw->conf.chandef.chan->band;
	u8 num_supported_rates = 0;
	u8 rate_table_offset, rate_offset = 0;
	u32 rate_bitmap;
	u16 *selected_rates, min_rate;
	bool is_ht = false, is_sgi = false;
	u16 frame_len = sizeof(struct rsi_auto_rate);

	rsi_dbg(MGMT_TX_ZONE,
		"%s: Sending auto rate request frame\n", __func__);

	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of skb\n",
			__func__);
		return -ENOMEM;
	}

	memset(skb->data, 0, frame_len);
	selected_rates = kzalloc(2 * RSI_TBL_SZ, GFP_KERNEL);
	if (!selected_rates) {
		rsi_dbg(ERR_ZONE, "%s: Failed in allocation of mem\n",
			__func__);
		dev_kfree_skb(skb);
		return -ENOMEM;
	}

	auto_rate = (struct rsi_auto_rate *)skb->data;

	auto_rate->aarf_rssi = cpu_to_le16(((u16)3 << 6) | (u16)(18 & 0x3f));
	auto_rate->collision_tolerance = cpu_to_le16(3);
	auto_rate->failure_limit = cpu_to_le16(3);
	auto_rate->initial_boundary = cpu_to_le16(3);
	auto_rate->max_threshold_limt = cpu_to_le16(27);

	auto_rate->desc.desc_dword0.frame_type = AUTO_RATE_IND;

	if (common->channel_width == BW_40MHZ)
		auto_rate->desc.desc_dword3.qid_tid = BW_40MHZ;
	auto_rate->desc.desc_dword3.sta_id = sta_id;

	if (vif->type == NL80211_IFTYPE_STATION) {
		rate_bitmap = common->bitrate_mask[band];
		is_ht = common->vif_info[0].is_ht;
		is_sgi = common->vif_info[0].sgi;
	} else {
		rate_bitmap = sta->supp_rates[band];
		is_ht = sta->ht_cap.ht_supported;
		if ((sta->ht_cap.cap & IEEE80211_HT_CAP_SGI_20) ||
		    (sta->ht_cap.cap & IEEE80211_HT_CAP_SGI_40))
			is_sgi = true;
	}

	if (band == NL80211_BAND_2GHZ) {
		if ((rate_bitmap == 0) && (is_ht))
			min_rate = RSI_RATE_MCS0;
		else
			min_rate = RSI_RATE_1;
		rate_table_offset = 0;
	} else {
		if ((rate_bitmap == 0) && (is_ht))
			min_rate = RSI_RATE_MCS0;
		else
			min_rate = RSI_RATE_6;
		rate_table_offset = 4;
	}

	for (ii = 0, jj = 0;
	     ii < (ARRAY_SIZE(rsi_rates) - rate_table_offset); ii++) {
		if (rate_bitmap & BIT(ii)) {
			selected_rates[jj++] =
			(rsi_rates[ii + rate_table_offset].bitrate / 5);
			rate_offset++;
		}
	}
	num_supported_rates = jj;

	if (is_ht) {
		for (ii = 0; ii < ARRAY_SIZE(mcs); ii++)
			selected_rates[jj++] = mcs[ii];
		num_supported_rates += ARRAY_SIZE(mcs);
		rate_offset += ARRAY_SIZE(mcs);
	}

	sort(selected_rates, jj, sizeof(u16), &rsi_compare, NULL);

	/* mapping the rates to RSI rates */
	for (ii = 0; ii < jj; ii++) {
		if (rsi_map_rates(selected_rates[ii], &kk)) {
			auto_rate->supported_rates[ii] =
				cpu_to_le16(rsi_rates[kk].hw_value);
		} else {
			auto_rate->supported_rates[ii] =
				cpu_to_le16(rsi_mcsrates[kk]);
		}
	}

	/* loading HT rates in the bottom half of the auto rate table */
	if (is_ht) {
		for (ii = rate_offset, kk = ARRAY_SIZE(rsi_mcsrates) - 1;
		     ii < rate_offset + 2 * ARRAY_SIZE(rsi_mcsrates); ii++) {
			if (is_sgi || conf_is_ht40(&common->priv->hw->conf))
				auto_rate->supported_rates[ii++] =
					cpu_to_le16(rsi_mcsrates[kk] | BIT(9));
			else
				auto_rate->supported_rates[ii++] =
					cpu_to_le16(rsi_mcsrates[kk]);
			auto_rate->supported_rates[ii] =
				cpu_to_le16(rsi_mcsrates[kk--]);
		}

		for (; ii < (RSI_TBL_SZ - 1); ii++) {
			auto_rate->supported_rates[ii] =
				cpu_to_le16(rsi_mcsrates[0]);
		}
	}

	for (; ii < RSI_TBL_SZ; ii++)
		auto_rate->supported_rates[ii] = cpu_to_le16(min_rate);

	auto_rate->num_supported_rates = cpu_to_le16(num_supported_rates * 2);
	auto_rate->moderate_rate_inx = cpu_to_le16(num_supported_rates / 2);
	num_supported_rates *= 2;

	rsi_set_len_qno(&auto_rate->desc.desc_dword0.len_qno,
			(frame_len - FRAME_DESC_SZ), RSI_WIFI_MGMT_Q);

	skb_put(skb, frame_len);
	kfree(selected_rates);

	return rsi_send_internal_mgmt_frame(common, skb);
}