status_t ProCamera2Client::exclusiveUnlock() {
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 Mutex::Autolock icl(mBinderSerializationLock);
 SharedCameraCallbacks::Lock l(mSharedCameraCallbacks);

 if (!mExclusiveLock) {
        ALOGW("%s: cannot unlock, no lock was held in the first place",
              __FUNCTION__);
 return BAD_VALUE;
 }

    mExclusiveLock = false;
 if (mRemoteCallback != NULL ) {
        mRemoteCallback->onLockStatusChanged(
 IProCameraCallbacks::LOCK_RELEASED);
 }
    ALOGV("%s: exclusive lock released", __FUNCTION__);

 return OK;
}
