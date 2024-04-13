ssize_t MediaPlayerService::AudioOutput::channelCount() const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return NO_INIT;
 return mTrack->channelCount();
}
