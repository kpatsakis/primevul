static jboolean configHciSnoopLogNative(JNIEnv* env, jobject obj, jboolean enable) {
    ALOGV("%s:",__FUNCTION__);

    jboolean result = JNI_FALSE;

 if (!sBluetoothInterface) return result;

 int ret = sBluetoothInterface->config_hci_snoop_log(enable);

    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
