void AudioFlinger::EffectChain::checkSuspendOnEffectEnabled(const sp<EffectModule>& effect,
 bool enabled)
{
 ssize_t index = mSuspendedEffects.indexOfKey(effect->desc().type.timeLow);
 if (enabled) {
 if (index < 0) {
            index = mSuspendedEffects.indexOfKey((int)kKeyForSuspendAll);
 if (index < 0) {
 return;
 }
 if (!isEffectEligibleForSuspend(effect->desc())) {
 return;
 }
            setEffectSuspended_l(&effect->desc().type, enabled);
            index = mSuspendedEffects.indexOfKey(effect->desc().type.timeLow);
 if (index < 0) {
                ALOGW("checkSuspendOnEffectEnabled() Fx should be suspended here!");
 return;
 }
 }
        ALOGV("checkSuspendOnEffectEnabled() enable suspending fx %08x",
            effect->desc().type.timeLow);
        sp<SuspendedEffectDesc> desc = mSuspendedEffects.valueAt(index);
 if (desc->mEffect == 0) {
            desc->mEffect = effect;
            effect->setEnabled(false);
            effect->setSuspended(true);
 }
 } else {
 if (index < 0) {
 return;
 }
        ALOGV("checkSuspendOnEffectEnabled() disable restoring fx %08x",
            effect->desc().type.timeLow);
        sp<SuspendedEffectDesc> desc = mSuspendedEffects.valueAt(index);
        desc->mEffect.clear();
        effect->setSuspended(false);
 }
}
