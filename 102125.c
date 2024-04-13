bool AudioFlinger::EffectModule::suspended() const
{
 Mutex::Autolock _l(mLock);
 return mSuspended;
}
