void Camera3Device::internalUpdateStatusLocked(Status status) {
    mStatus = status;
    mRecentStatusUpdates.add(mStatus);
    mStatusChanged.broadcast();
}
