status_t CameraClient::startRecording() {
    LOG1("startRecording (pid %d)", getCallingPid());
 return startCameraMode(CAMERA_RECORDING_MODE);
}
