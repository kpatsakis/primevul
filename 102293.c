status_t CameraClient::unlock() {
 int callingPid = getCallingPid();
    LOG1("unlock (pid %d)", callingPid);
 Mutex::Autolock lock(mLock);

 status_t result = checkPid();
 if (result == NO_ERROR) {
 if (mHardware->recordingEnabled()) {
            ALOGE("Not allowed to unlock camera during recording.");
 return INVALID_OPERATION;
 }
        mClientPid = 0;
        LOG1("clear mRemoteCallback (pid %d)", callingPid);
        mRemoteCallback.clear();
 }
 return result;
}
