static void on_tdls_state_changed(mac_addr addr, wifi_tdls_status status) {

 JNIHelper helper(mVM);

    ALOGD("on_tdls_state_changed is called: vm = %p, obj = %p", mVM, mCls);

 char mac[32];
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1], addr[2], addr[3], addr[4],
            addr[5]);

 JNIObject<jstring> mac_address = helper.newStringUTF(mac);
    helper.reportEvent(mCls, "onTdlsStatus", "(Ljava/lang/StringII;)V",
        mac_address.get(), status.state, status.reason);

}
