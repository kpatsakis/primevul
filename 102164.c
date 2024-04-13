static jboolean android_net_wifi_setBssidBlacklist(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject list) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("configure BSSID black list request [%d] = %p", id, handle);

    wifi_bssid_params params;
    memset(&params, 0, sizeof(params));

 if (list != NULL) {
 size_t len = helper.getArrayLength((jobjectArray)list);
 if (len > (size_t)MAX_BLACKLIST_BSSID) {
 return false;
 }
 for (unsigned int i = 0; i < len; i++) {

 JNIObject<jobject> jbssid = helper.getObjectArrayElement(list, i);
 if (jbssid == NULL) {
                ALOGD("configure BSSID blacklist: could not get element %d", i);
 continue;
 }

 ScopedUtfChars chars(env, (jstring)jbssid.get());
 const char *bssid = chars.c_str();
 if (bssid == NULL) {
                ALOGE("Error getting bssid");
 return false;
 }

            mac_addr addr;
            parseMacAddress(bssid, addr);
            memcpy(params.bssids[i], addr, sizeof(mac_addr));

 char bssidOut[32];
            sprintf(bssidOut, "%0x:%0x:%0x:%0x:%0x:%0x", addr[0], addr[1],
                addr[2], addr[3], addr[4], addr[5]);

            ALOGD("BSSID blacklist: added bssid %s", bssidOut);

            params.num_bssid++;
 }
 }

    ALOGD("Added %d bssids", params.num_bssid);
 return hal_fn.wifi_set_bssid_blacklist(id, handle, params) == WIFI_SUCCESS;
}
