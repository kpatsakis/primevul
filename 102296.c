status_t CameraDeviceClient::flush() {
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 status_t res = OK;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 return mDevice->flush();

 }
