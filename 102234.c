bool Camera3Device::RequestThread::threadLoop() {
    ATRACE_CALL();
 status_t res;

 if (waitIfPaused()) {
 return true;
 }

    waitForNextRequestBatch();
 if (mNextRequests.size() == 0) {
 return true;
 }

 int latestRequestId;
 camera_metadata_entry_t requestIdEntry = mNextRequests[mNextRequests.size() - 1].
            captureRequest->mSettings.find(ANDROID_REQUEST_ID);
 if (requestIdEntry.count > 0) {
        latestRequestId = requestIdEntry.data.i32[0];
 } else {
        ALOGW("%s: Did not have android.request.id set in the request.", __FUNCTION__);
        latestRequestId = NAME_NOT_FOUND;
 }

    res = prepareHalRequests();
 if (res == TIMED_OUT) {
        cleanUpFailedRequests(/*sendRequestError*/ true);
 return true;
 } else if (res != OK) {
        cleanUpFailedRequests(/*sendRequestError*/ false);
 return false;
 }

 {
 Mutex::Autolock al(mLatestRequestMutex);

        mLatestRequestId = latestRequestId;
        mLatestRequestSignal.signal();
 }

 bool useFlushLock = mNextRequests.size() > 1;

 if (useFlushLock) {
        mFlushLock.lock();
 }

    ALOGVV("%s: %d: submitting %d requests in a batch.", __FUNCTION__, __LINE__,
            mNextRequests.size());
 for (auto& nextRequest : mNextRequests) {
        ATRACE_ASYNC_BEGIN("frame capture", nextRequest.halRequest.frame_number);
        ATRACE_BEGIN("camera3->process_capture_request");
        res = mHal3Device->ops->process_capture_request(mHal3Device, &nextRequest.halRequest);
        ATRACE_END();

 if (res != OK) {
            SET_ERR("RequestThread: Unable to submit capture request %d to HAL"
 " device: %s (%d)", nextRequest.halRequest.frame_number, strerror(-res),
                    res);
            cleanUpFailedRequests(/*sendRequestError*/ false);
 if (useFlushLock) {
                mFlushLock.unlock();
 }
 return false;
 }

        nextRequest.submitted = true;

 if (nextRequest.halRequest.settings != NULL) { // Don't update if they were unchanged
 Mutex::Autolock al(mLatestRequestMutex);

 camera_metadata_t* cloned = clone_camera_metadata(nextRequest.halRequest.settings);
            mLatestRequest.acquire(cloned);
 }

 if (nextRequest.halRequest.settings != NULL) {
            nextRequest.captureRequest->mSettings.unlock(nextRequest.halRequest.settings);
 }

        res = removeTriggers(mPrevRequest);
 if (res != OK) {
            SET_ERR("RequestThread: Unable to remove triggers "
 "(capture request %d, HAL device: %s (%d)",
                  nextRequest.halRequest.frame_number, strerror(-res), res);
            cleanUpFailedRequests(/*sendRequestError*/ false);
 if (useFlushLock) {
                mFlushLock.unlock();
 }
 return false;
 }
 }

 if (useFlushLock) {
        mFlushLock.unlock();
 }

 {
 Mutex::Autolock l(mRequestLock);
        mNextRequests.clear();
 }

 return true;
}
