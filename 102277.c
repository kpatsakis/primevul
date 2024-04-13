status_t CameraService::getCameraCharacteristics(int cameraId,
 CameraMetadata* cameraInfo) {
 if (!cameraInfo) {
        ALOGE("%s: cameraInfo is NULL", __FUNCTION__);
 return BAD_VALUE;
 }

 if (!mModule) {
        ALOGE("%s: camera hardware module doesn't exist", __FUNCTION__);
 return -ENODEV;
 }

 if (mModule->common.module_api_version < CAMERA_MODULE_API_VERSION_2_0) {
        ALOGE("%s: Only HAL module version V2 or higher supports static metadata", __FUNCTION__);
 return BAD_VALUE;
 }

 if (cameraId < 0 || cameraId >= mNumberOfCameras) {
        ALOGE("%s: Invalid camera id: %d", __FUNCTION__, cameraId);
 return BAD_VALUE;
 }

 int facing;
 if (getDeviceVersion(cameraId, &facing) == CAMERA_DEVICE_API_VERSION_1_0) {
        ALOGE("%s: HAL1 doesn't support static metadata yet", __FUNCTION__);
 return BAD_VALUE;
 }

 if (getDeviceVersion(cameraId, &facing) <= CAMERA_DEVICE_API_VERSION_2_1) {
        ALOGW("%s: HAL2.x doesn't support getCameraCharacteristics for now", __FUNCTION__);
 return BAD_VALUE;
 }

 struct camera_info info;
 status_t ret = mModule->get_camera_info(cameraId, &info);
 *cameraInfo = info.static_camera_characteristics;

 return ret;
}
