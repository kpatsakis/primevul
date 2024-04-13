CameraService::~CameraService() {
 for (int i = 0; i < mNumberOfCameras; i++) {
 if (mBusy[i]) {
            ALOGE("camera %d is still in use in destructor!", i);
 }
 }

    gCameraService = NULL;
}
