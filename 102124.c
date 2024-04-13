size_t AudioFlinger::EffectChain::removeEffect_l(const sp<EffectModule>& effect)
{
 Mutex::Autolock _l(mLock);
 size_t size = mEffects.size();
 uint32_t type = effect->desc().flags & EFFECT_FLAG_TYPE_MASK;

 for (size_t i = 0; i < size; i++) {
 if (effect == mEffects[i]) {
 if (mEffects[i]->state() == EffectModule::ACTIVE ||
                    mEffects[i]->state() == EffectModule::STOPPING) {
                mEffects[i]->stop();
 }
 if (type == EFFECT_FLAG_TYPE_AUXILIARY) {
 delete[] effect->inBuffer();
 } else {
 if (i == size - 1 && i != 0) {
                    mEffects[i - 1]->setOutBuffer(mOutBuffer);
                    mEffects[i - 1]->configure();
 }
 }
            mEffects.removeAt(i);
            ALOGV("removeEffect_l() effect %p, removed from chain %p at rank %zu", effect.get(),
 this, i);
 break;
 }
 }

 return mEffects.size();
}
