static jint android_net_wifi_start_rssi_monitoring_native(JNIEnv *env, jclass cls, jint iface,
        jint idx, jbyte maxRssi, jbyte minRssi) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("Start Rssi monitoring = %p", handle);
    ALOGD("MinRssi %d MaxRssi %d", minRssi, maxRssi);
    wifi_error ret;
    wifi_request_id id = idx;
    wifi_rssi_event_handler eh;
    eh.on_rssi_threshold_breached = onRssiThresholdbreached;
    ret = hal_fn.wifi_start_rssi_monitoring(id, handle, maxRssi, minRssi, eh);
 return ret;
}
