String8 Camera2Client::getParameters() const {
    ATRACE_CALL();
    ALOGV("%s: Camera %d", __FUNCTION__, mCameraId);
 Mutex::Autolock icl(mBinderSerializationLock);
 if ( checkPid(__FUNCTION__) != OK) return String8();

 SharedParameters::ReadLock l(mParameters);

 return l.mParameters.get();
}
