audio_session_t MediaPlayerService::AudioOutput::getSessionId() const
{
 Mutex::Autolock lock(mLock);
 return mSessionId;
}
