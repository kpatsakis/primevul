static jintArray android_net_wifi_getValidChannels(JNIEnv *env, jclass cls,
        jint iface, jint band) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("getting valid channels %p", handle);

 static const int MaxChannels = 64;
    wifi_channel channels[64];
 int num_channels = 0;
    wifi_error result = hal_fn.wifi_get_valid_channels(handle, band, MaxChannels,
            channels, &num_channels);

 if (result == WIFI_SUCCESS) {
 JNIObject<jintArray> channelArray = helper.newIntArray(num_channels);
 if (channelArray == NULL) {
            ALOGE("failed to allocate channel list");
 return NULL;
 }

        helper.setIntArrayRegion(channelArray, 0, num_channels, channels);
 return channelArray.detach();
 } else {
        ALOGE("failed to get channel list : %d", result);
 return NULL;
 }
}
