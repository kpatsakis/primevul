static jboolean android_net_wifi_setHotlist(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject ap) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("setting hotlist on interface[%d] = %p", iface, handle);

    wifi_bssid_hotlist_params params;
    memset(&params, 0, sizeof(params));

    params.lost_ap_sample_size = helper.getIntField(ap, "apLostThreshold");

 JNIObject<jobjectArray> array = helper.getArrayField(
            ap, "bssidInfos", "[Landroid/net/wifi/WifiScanner$BssidInfo;");
    params.num_bssid = helper.getArrayLength(array);

 if (params.num_bssid == 0) {
        ALOGE("Error in accesing array");
 return false;
 }

 for (int i = 0; i < params.num_bssid; i++) {
 JNIObject<jobject> objAp = helper.getObjectArrayElement(array, i);

 JNIObject<jstring> macAddrString = helper.getStringField(objAp, "bssid");
 if (macAddrString == NULL) {
            ALOGE("Error getting bssid field");
 return false;
 }

 ScopedUtfChars chars(env, macAddrString);
 const char *bssid = chars.c_str();
 if (bssid == NULL) {
            ALOGE("Error getting bssid");
 return false;
 }
        parseMacAddress(bssid, params.ap[i].bssid);

        mac_addr addr;
        memcpy(addr, params.ap[i].bssid, sizeof(mac_addr));

 char bssidOut[32];
        sprintf(bssidOut, "%0x:%0x:%0x:%0x:%0x:%0x", addr[0], addr[1],
            addr[2], addr[3], addr[4], addr[5]);

        ALOGD("Added bssid %s", bssidOut);

        params.ap[i].low = helper.getIntField(objAp, "low");
        params.ap[i].high = helper.getIntField(objAp, "high");
 }

    wifi_hotlist_ap_found_handler handler;
    memset(&handler, 0, sizeof(handler));

    handler.on_hotlist_ap_found = &onHotlistApFound;
    handler.on_hotlist_ap_lost  = &onHotlistApLost;
 return hal_fn.wifi_set_bssid_hotlist(id, handle, params, handler) == WIFI_SUCCESS;
}
