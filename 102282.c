status_t Camera2Client::connect(const sp<ICameraClient>& client) {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);
 Mutex::Autolock icl(mBinderSerializationLock);

 if (mClientPid != 0 && getCallingPid() != mClientPid) {
        ALOGE("%s: Camera %d: Connection attempt from pid %d; "
 "current locked to pid %d", __FUNCTION__,
                mCameraId, getCallingPid(), mClientPid);
 return BAD_VALUE;
 }

    mClientPid = getCallingPid();

    mRemoteCallback = client;
    mSharedCameraCallbacks = client;

 return OK;
}
