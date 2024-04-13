AudioFlinger::EffectHandle::~EffectHandle()
{
    ALOGV("Destructor %p", this);

 if (mEffect == 0) {
        mDestroyed = true;
 return;
 }
    mEffect->lock();
    mDestroyed = true;
    mEffect->unlock();
    disconnect(false);
}
