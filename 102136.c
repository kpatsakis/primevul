void MediaPlayerService::AudioOutput::stop()
{
    ALOGV("stop");
 Mutex::Autolock lock(mLock);
 if (mTrack != 0) mTrack->stop();
}
