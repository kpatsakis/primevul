status_t OMXCodec::pause() {
 Mutex::Autolock autoLock(mLock);

    mPaused = true;

 return OK;
}
