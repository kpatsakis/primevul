static jobject android_net_wifi_getScanResults(
 JNIEnv *env, jclass cls, jint iface, jboolean flush) {

 JNIHelper helper(env);
    wifi_cached_scan_results scan_data[64];
 int num_scan_data = 64;

    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    byte b = flush ? 0xFF : 0;
 int result = hal_fn.wifi_get_cached_gscan_results(handle, b, num_scan_data, scan_data, &num_scan_data);
 if (result == WIFI_SUCCESS) {
 JNIObject<jobjectArray> scanData = helper.createObjectArray(
 "android/net/wifi/WifiScanner$ScanData", num_scan_data);
 if (scanData == NULL) {
            ALOGE("Error in allocating array of scanData");
 return NULL;
 }

 for (int i = 0; i < num_scan_data; i++) {

 JNIObject<jobject> data = helper.createObject("android/net/wifi/WifiScanner$ScanData");
 if (data == NULL) {
                ALOGE("Error in allocating scanData");
 return NULL;
 }

            helper.setIntField(data, "mId", scan_data[i].scan_id);
            helper.setIntField(data, "mFlags", scan_data[i].flags);

 /* sort all scan results by timestamp */
            qsort(scan_data[i].results, scan_data[i].num_results,
 sizeof(wifi_scan_result), compare_scan_result_timestamp);

 JNIObject<jobjectArray> scanResults = helper.createObjectArray(
 "android/net/wifi/ScanResult", scan_data[i].num_results);
 if (scanResults == NULL) {
                ALOGE("Error in allocating scanResult array");
 return NULL;
 }

            wifi_scan_result *results = scan_data[i].results;
 for (int j = 0; j < scan_data[i].num_results; j++) {

 JNIObject<jobject> scanResult = createScanResult(helper, &results[j]);
 if (scanResult == NULL) {
                    ALOGE("Error in creating scan result");
 return NULL;
 }

                helper.setObjectArrayElement(scanResults, j, scanResult);
 }

            helper.setObjectField(data, "mResults", "[Landroid/net/wifi/ScanResult;", scanResults);
            helper.setObjectArrayElement(scanData, i, data);
 }

 return scanData.detach();
 } else {
 return NULL;
 }
}
