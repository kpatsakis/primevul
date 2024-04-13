static jobject android_net_wifi_getLinkLayerStats (JNIEnv *env, jclass cls, jint iface) {

 JNIHelper helper(env);
    wifi_stats_result_handler handler;
    memset(&handler, 0, sizeof(handler));
    handler.on_link_stats_results = &onLinkStatsResults;
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
 int result = hal_fn.wifi_get_link_stats(0, handle, handler);
 if (result < 0) {
        ALOGE("android_net_wifi_getLinkLayerStats: failed to get link statistics\n");
 return NULL;
 }

 JNIObject<jobject> wifiLinkLayerStats = helper.createObject(
 "android/net/wifi/WifiLinkLayerStats");
 if (wifiLinkLayerStats == NULL) {
       ALOGE("Error in allocating wifiLinkLayerStats");
 return NULL;
 }

    helper.setIntField(wifiLinkLayerStats, "beacon_rx", link_stat.beacon_rx);
    helper.setIntField(wifiLinkLayerStats, "rssi_mgmt", link_stat.rssi_mgmt);
    helper.setLongField(wifiLinkLayerStats, "rxmpdu_be", link_stat.ac[WIFI_AC_BE].rx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "rxmpdu_bk", link_stat.ac[WIFI_AC_BK].rx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "rxmpdu_vi", link_stat.ac[WIFI_AC_VI].rx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "rxmpdu_vo", link_stat.ac[WIFI_AC_VO].rx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "txmpdu_be", link_stat.ac[WIFI_AC_BE].tx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "txmpdu_bk", link_stat.ac[WIFI_AC_BK].tx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "txmpdu_vi", link_stat.ac[WIFI_AC_VI].tx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "txmpdu_vo", link_stat.ac[WIFI_AC_VO].tx_mpdu);
    helper.setLongField(wifiLinkLayerStats, "lostmpdu_be", link_stat.ac[WIFI_AC_BE].mpdu_lost);
    helper.setLongField(wifiLinkLayerStats, "lostmpdu_bk", link_stat.ac[WIFI_AC_BK].mpdu_lost);
    helper.setLongField(wifiLinkLayerStats, "lostmpdu_vi",  link_stat.ac[WIFI_AC_VI].mpdu_lost);
    helper.setLongField(wifiLinkLayerStats, "lostmpdu_vo", link_stat.ac[WIFI_AC_VO].mpdu_lost);
    helper.setLongField(wifiLinkLayerStats, "retries_be", link_stat.ac[WIFI_AC_BE].retries);
    helper.setLongField(wifiLinkLayerStats, "retries_bk", link_stat.ac[WIFI_AC_BK].retries);
    helper.setLongField(wifiLinkLayerStats, "retries_vi", link_stat.ac[WIFI_AC_VI].retries);
    helper.setLongField(wifiLinkLayerStats, "retries_vo", link_stat.ac[WIFI_AC_VO].retries);

    helper.setIntField(wifiLinkLayerStats, "on_time", radio_stat.on_time);
    helper.setIntField(wifiLinkLayerStats, "tx_time", radio_stat.tx_time);
    helper.setIntField(wifiLinkLayerStats, "rx_time", radio_stat.rx_time);
    helper.setIntField(wifiLinkLayerStats, "on_time_scan", radio_stat.on_time_scan);

 return wifiLinkLayerStats.detach();
}
