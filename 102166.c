static void android_net_wifi_setLinkLayerStats (JNIEnv *env, jclass cls, jint iface, int enable) {
 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    wifi_link_layer_params params;
    params.aggressive_statistics_gathering = enable;
    params.mpdu_size_threshold = 128;

    ALOGD("android_net_wifi_setLinkLayerStats: %u\n", enable);

    hal_fn.wifi_set_link_stats(handle, params);
}
