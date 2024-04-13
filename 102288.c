status_t CameraClient::checkPid() const {
 int callingPid = getCallingPid();
 if (callingPid == mClientPid) return NO_ERROR;

    ALOGW("attempt to use a locked camera from a different process"
 " (old pid %d, new pid %d)", mClientPid, callingPid);
 return EBUSY;
}
