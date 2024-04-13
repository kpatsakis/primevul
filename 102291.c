status_t CameraClient::setParameters(const String8& params) {
    LOG1("setParameters (pid %d) (%s)", getCallingPid(), params.string());

 Mutex::Autolock lock(mLock);
 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 CameraParameters p(params);
 return mHardware->setParameters(p);
}
