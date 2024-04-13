status_t MediaPlayerService::Client::setPlaybackSettings(const AudioPlaybackRate& rate)
{
    ALOGV("[%d] setPlaybackSettings(%f, %f, %d, %d)",
            mConnId, rate.mSpeed, rate.mPitch, rate.mFallbackMode, rate.mStretchMode);
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 return p->setPlaybackSettings(rate);
}
