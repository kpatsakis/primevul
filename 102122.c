sp<AudioFlinger::EffectModule> AudioFlinger::EffectChain::getEffectFromId_l(int id)
{
 size_t size = mEffects.size();

 for (size_t i = 0; i < size; i++) {
 if (id == 0 || mEffects[i]->id() == id) {
 return mEffects[i];
 }
 }
 return 0;
}
