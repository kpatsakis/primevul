static jint android_net_wifi_stop_rssi_monitoring_native(JNIEnv *env, jclass cls,
        jint iface, jint idx) {
 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("Stop Rssi monitoring = %p", handle);
    wifi_error ret;
    wifi_request_id id = idx;
    ret = hal_fn.wifi_stop_rssi_monitoring(id, handle);
 return ret;
}
