status_t CameraDeviceClient::cancelRequest(int requestId) {
    ATRACE_CALL();
    ALOGV("%s, requestId = %d", __FUNCTION__, requestId);

 status_t res;

 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 Vector<int>::iterator it, end;
 for (it = mStreamingRequestList.begin(), end = mStreamingRequestList.end();
         it != end; ++it) {
 if (*it == requestId) {
 break;
 }
 }

 if (it == end) {
        ALOGE("%s: Camera%d: Did not find request id %d in list of streaming "
 "requests", __FUNCTION__, mCameraId, requestId);
 return BAD_VALUE;
 }

    res = mDevice->clearStreamingRequest();

 if (res == OK) {
        ALOGV("%s: Camera %d: Successfully cleared streaming request",
              __FUNCTION__, mCameraId);
        mStreamingRequestList.erase(it);
 }

 return res;
}
