static void acl_state_changed_callback(bt_status_t status, bt_bdaddr_t *bd_addr,
 bt_acl_state_t state)
{
    jbyteArray addr;
 int i;
 if (!checkCallbackThread()) {
       ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }
 if (!bd_addr) {
        ALOGE("Address is null in %s", __FUNCTION__);
 return;
 }
    addr = callbackEnv->NewByteArray(sizeof(bt_bdaddr_t));
 if (addr == NULL) {
       ALOGE("Address allocation failed in %s", __FUNCTION__);
 return;
 }
    callbackEnv->SetByteArrayRegion(addr, 0, sizeof(bt_bdaddr_t), (jbyte *)bd_addr);

    callbackEnv->CallVoidMethod(sJniCallbacksObj, method_aclStateChangeCallback, (jint) status,
                                addr, (jint)state);
    checkAndClearExceptionFromCallback(callbackEnv, __FUNCTION__);
    callbackEnv->DeleteLocalRef(addr);
}
