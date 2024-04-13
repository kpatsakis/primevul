void MediaPlayerService::Client::setDataSource_post(
 const sp<MediaPlayerBase>& p,
 status_t status)
{
    ALOGV(" setDataSource");
    mStatus = status;
 if (mStatus != OK) {
        ALOGE("  error: %d", mStatus);
 return;
 }

 if (mRetransmitEndpointValid) {
        mStatus = p->setRetransmitEndpoint(&mRetransmitEndpoint);
 if (mStatus != NO_ERROR) {
            ALOGE("setRetransmitEndpoint error: %d", mStatus);
 }
 }

 if (mStatus == OK) {
        mPlayer = p;
 }
}
