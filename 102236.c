status_t Camera3Device::waitUntilStateThenRelock(bool active, nsecs_t timeout) {
 status_t res = OK;

 size_t startIndex = 0;
 if (mStatusWaiters == 0) {
        mRecentStatusUpdates.clear();
 } else {
        startIndex = mRecentStatusUpdates.size();
 }

    mStatusWaiters++;

 bool stateSeen = false;
 do {
 if (active == (mStatus == STATUS_ACTIVE)) {
 break;
 }

        res = mStatusChanged.waitRelative(mLock, timeout);
 if (res != OK) break;

        LOG_ALWAYS_FATAL_IF(startIndex > mRecentStatusUpdates.size(),
 "%s: Skipping status updates in Camera3Device, may result in deadlock.",
                __FUNCTION__);

 for (size_t i = startIndex; i < mRecentStatusUpdates.size(); i++) {
 if (active == (mRecentStatusUpdates[i] == STATUS_ACTIVE) ) {
                stateSeen = true;
 break;
 }
 }
 } while (!stateSeen);

    mStatusWaiters--;

 return res;
}
