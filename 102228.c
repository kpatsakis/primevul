status_t Camera3Device::RequestThread::clear(
 NotificationListener *listener,
 /*out*/int64_t *lastFrameNumber) {
 Mutex::Autolock l(mRequestLock);
    ALOGV("RequestThread::%s:", __FUNCTION__);

    mRepeatingRequests.clear();

 if (listener != NULL) {
 for (RequestList::iterator it = mRequestQueue.begin();
                 it != mRequestQueue.end(); ++it) {
 if ((*it)->mInputStream != NULL) {
 camera3_stream_buffer_t inputBuffer;
 status_t res = (*it)->mInputStream->getInputBuffer(&inputBuffer);
 if (res != OK) {
                    ALOGW("%s: %d: couldn't get input buffer while clearing the request "
 "list: %s (%d)", __FUNCTION__, __LINE__, strerror(-res), res);
 } else {
                    res = (*it)->mInputStream->returnInputBuffer(inputBuffer);
 if (res != OK) {
                        ALOGE("%s: %d: couldn't return input buffer while clearing the request "
 "list: %s (%d)", __FUNCTION__, __LINE__, strerror(-res), res);
 }
 }
 }
 (*it)->mResultExtras.frameNumber = mFrameNumber++;
            listener->notifyError(ICameraDeviceCallbacks::ERROR_CAMERA_REQUEST,
 (*it)->mResultExtras);
 }
 }
    mRequestQueue.clear();
    mTriggerMap.clear();
 if (lastFrameNumber != NULL) {
 *lastFrameNumber = mRepeatingLastFrameNumber;
 }
    mRepeatingLastFrameNumber = NO_IN_FLIGHT_REPEATING_FRAMES;
 return OK;
}
