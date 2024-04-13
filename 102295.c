bool CameraDeviceClient::enforceRequestPermissions(CameraMetadata& metadata) {

 const int pid = IPCThreadState::self()->getCallingPid();
 const int selfPid = getpid();
 camera_metadata_entry_t entry;

 /**
     * Mixin default important security values
     * - android.led.transmit = defaulted ON
     */
 CameraMetadata staticInfo = mDevice->info();
    entry = staticInfo.find(ANDROID_LED_AVAILABLE_LEDS);
 for(size_t i = 0; i < entry.count; ++i) {
 uint8_t led = entry.data.u8[i];

 switch(led) {
 case ANDROID_LED_AVAILABLE_LEDS_TRANSMIT: {
 uint8_t transmitDefault = ANDROID_LED_TRANSMIT_ON;
 if (!metadata.exists(ANDROID_LED_TRANSMIT)) {
                    metadata.update(ANDROID_LED_TRANSMIT,
 &transmitDefault, 1);
 }
 break;
 }
 }
 }

 if (pid == selfPid) {
 return true;
 }

 /**
     * Permission check special fields in the request
     * - android.led.transmit = android.permission.CAMERA_DISABLE_TRANSMIT
     */
    entry = metadata.find(ANDROID_LED_TRANSMIT);
 if (entry.count > 0 && entry.data.u8[0] != ANDROID_LED_TRANSMIT_ON) {
 String16 permissionString =
 String16("android.permission.CAMERA_DISABLE_TRANSMIT_LED");
 if (!checkCallingPermission(permissionString)) {
 const int uid = IPCThreadState::self()->getCallingUid();
            ALOGE("Permission Denial: "
 "can't disable transmit LED pid=%d, uid=%d", pid, uid);
 return false;
 }
 }

 return true;
}
