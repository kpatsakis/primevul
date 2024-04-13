status_t Camera2Client::commandStopFaceDetectionL(Parameters &params) {
 status_t res = OK;
    ALOGV("%s: Camera %d: Stopping face detection",
          __FUNCTION__, mCameraId);

 if (!params.enableFaceDetect) return OK;

    params.enableFaceDetect = false;

 if (params.state == Parameters::PREVIEW
 || params.state == Parameters::RECORD
 || params.state == Parameters::VIDEO_SNAPSHOT) {
        res = updateRequests(params);
 }

 return res;
}
