void CameraService::releaseSound() {
 Mutex::Autolock lock(mSoundLock);
    LOG1("CameraService::releaseSound ref=%d", mSoundRef);
 if (--mSoundRef) return;

 for (int i = 0; i < NUM_SOUNDS; i++) {
 if (mSoundPlayer[i] != 0) {
            mSoundPlayer[i]->disconnect();
            mSoundPlayer[i].clear();
 }
 }
}
