void AudioFlinger::EffectChain::getSuspendEligibleEffects(
 Vector< sp<AudioFlinger::EffectModule> > &effects)
{
    effects.clear();
 for (size_t i = 0; i < mEffects.size(); i++) {
 if (isEffectEligibleForSuspend(mEffects[i]->desc())) {
            effects.add(mEffects[i]);
 }
 }
}
