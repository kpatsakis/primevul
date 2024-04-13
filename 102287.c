status_t Camera2Client::takePicture(int msgType) {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 int takePictureCounter;
 {
 SharedParameters::Lock l(mParameters);
 switch (l.mParameters.state) {
 case Parameters::DISCONNECTED:
 case Parameters::STOPPED:
 case Parameters::WAITING_FOR_PREVIEW_WINDOW:
                ALOGE("%s: Camera %d: Cannot take picture without preview enabled",
                        __FUNCTION__, mCameraId);
 return INVALID_OPERATION;
 case Parameters::PREVIEW:
                res = commandStopFaceDetectionL(l.mParameters);
 if (res != OK) {
                    ALOGE("%s: Camera %d: Unable to stop face detection for still capture",
                            __FUNCTION__, mCameraId);
 return res;
 }
                l.mParameters.state = Parameters::STILL_CAPTURE;
 break;
 case Parameters::RECORD:
                l.mParameters.state = Parameters::VIDEO_SNAPSHOT;
 break;
 case Parameters::STILL_CAPTURE:
 case Parameters::VIDEO_SNAPSHOT:
                ALOGE("%s: Camera %d: Already taking a picture",
                        __FUNCTION__, mCameraId);
 return INVALID_OPERATION;
 }

        ALOGV("%s: Camera %d: Starting picture capture", __FUNCTION__, mCameraId);

        res = updateProcessorStream(mJpegProcessor, l.mParameters);
 if (res != OK) {
            ALOGE("%s: Camera %d: Can't set up still image stream: %s (%d)",
                    __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }
        takePictureCounter = ++l.mParameters.takePictureCounter;
 }

    ATRACE_ASYNC_BEGIN(kTakepictureLabel, takePictureCounter);

    syncWithDevice();

    res = mCaptureSequencer->startCapture(msgType);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to start capture: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 }

 return res;
}
