bool Camera3Device::insert3AResult(CameraMetadata& result, int32_t tag,
 const T* value, uint32_t frameNumber) {
 if (result.update(tag, value, 1) != NO_ERROR) {
        mResultQueue.erase(--mResultQueue.end(), mResultQueue.end());
        SET_ERR("Frame %d: Failed to set %s in partial metadata",
                frameNumber, get_camera_metadata_tag_name(tag));
 return false;
 }
 return true;
}
