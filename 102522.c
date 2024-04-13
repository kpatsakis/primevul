struct sta_info *sta_info_alloc(struct ieee80211_sub_if_data *sdata,
				const u8 *addr, gfp_t gfp)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	struct timespec uptime;
	struct ieee80211_tx_latency_bin_ranges *tx_latency;
	int i;

	sta = kzalloc(sizeof(*sta) + local->hw.sta_data_size, gfp);
	if (!sta)
		return NULL;

	rcu_read_lock();
	tx_latency = rcu_dereference(local->tx_latency);
	/* init stations Tx latency statistics && TID bins */
	if (tx_latency) {
		sta->tx_lat = kzalloc(IEEE80211_NUM_TIDS *
				      sizeof(struct ieee80211_tx_latency_stat),
				      GFP_ATOMIC);
		if (!sta->tx_lat) {
			rcu_read_unlock();
			goto free;
		}

		if (tx_latency->n_ranges) {
			for (i = 0; i < IEEE80211_NUM_TIDS; i++) {
				/* size of bins is size of the ranges +1 */
				sta->tx_lat[i].bin_count =
					tx_latency->n_ranges + 1;
				sta->tx_lat[i].bins =
					kcalloc(sta->tx_lat[i].bin_count,
						sizeof(u32), GFP_ATOMIC);
				if (!sta->tx_lat[i].bins) {
					rcu_read_unlock();
					goto free;
				}
			}
		}
	}
 	rcu_read_unlock();
 
 	spin_lock_init(&sta->lock);
	spin_lock_init(&sta->ps_lock);
 	INIT_WORK(&sta->drv_unblock_wk, sta_unblock);
 	INIT_WORK(&sta->ampdu_mlme.work, ieee80211_ba_session_work);
 	mutex_init(&sta->ampdu_mlme.mtx);
#ifdef CONFIG_MAC80211_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif) &&
	    !sdata->u.mesh.user_mpm)
		init_timer(&sta->plink_timer);
	sta->nonpeer_pm = NL80211_MESH_POWER_ACTIVE;
#endif

	memcpy(sta->sta.addr, addr, ETH_ALEN);
	sta->local = local;
	sta->sdata = sdata;
	sta->last_rx = jiffies;

	sta->sta_state = IEEE80211_STA_NONE;

	do_posix_clock_monotonic_gettime(&uptime);
	sta->last_connected = uptime.tv_sec;
	ewma_init(&sta->avg_signal, 1024, 8);
	for (i = 0; i < ARRAY_SIZE(sta->chain_signal_avg); i++)
		ewma_init(&sta->chain_signal_avg[i], 1024, 8);

	if (sta_prepare_rate_control(local, sta, gfp))
		goto free;

	for (i = 0; i < IEEE80211_NUM_TIDS; i++) {
		/*
		 * timer_to_tid must be initialized with identity mapping
		 * to enable session_timer's data differentiation. See
		 * sta_rx_agg_session_timer_expired for usage.
		 */
		sta->timer_to_tid[i] = i;
	}
	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		skb_queue_head_init(&sta->ps_tx_buf[i]);
		skb_queue_head_init(&sta->tx_filtered[i]);
	}

	for (i = 0; i < IEEE80211_NUM_TIDS; i++)
		sta->last_seq_ctrl[i] = cpu_to_le16(USHRT_MAX);

	sta->sta.smps_mode = IEEE80211_SMPS_OFF;
	if (sdata->vif.type == NL80211_IFTYPE_AP ||
	    sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
		struct ieee80211_supported_band *sband =
			local->hw.wiphy->bands[ieee80211_get_sdata_band(sdata)];
		u8 smps = (sband->ht_cap.cap & IEEE80211_HT_CAP_SM_PS) >>
				IEEE80211_HT_CAP_SM_PS_SHIFT;
		/*
		 * Assume that hostapd advertises our caps in the beacon and
		 * this is the known_smps_mode for a station that just assciated
		 */
		switch (smps) {
		case WLAN_HT_SMPS_CONTROL_DISABLED:
			sta->known_smps_mode = IEEE80211_SMPS_OFF;
			break;
		case WLAN_HT_SMPS_CONTROL_STATIC:
			sta->known_smps_mode = IEEE80211_SMPS_STATIC;
			break;
		case WLAN_HT_SMPS_CONTROL_DYNAMIC:
			sta->known_smps_mode = IEEE80211_SMPS_DYNAMIC;
			break;
		default:
			WARN_ON(1);
		}
	}

	sta_dbg(sdata, "Allocated STA %pM\n", sta->sta.addr);
	return sta;

free:
	if (sta->tx_lat) {
		for (i = 0; i < IEEE80211_NUM_TIDS; i++)
			kfree(sta->tx_lat[i].bins);
		kfree(sta->tx_lat);
	}
	kfree(sta);
	return NULL;
}
