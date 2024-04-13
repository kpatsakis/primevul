int32_t SoftAAC2::outputDelayRingBufferGetSamples(INT_PCM *samples, int32_t numSamples) {

 if (numSamples > mOutputDelayRingBufferFilled) {
        ALOGE("RING BUFFER WOULD UNDERRUN");
 return -1;
 }

 if (mOutputDelayRingBufferReadPos + numSamples <= mOutputDelayRingBufferSize
 && (mOutputDelayRingBufferWritePos < mOutputDelayRingBufferReadPos
 || mOutputDelayRingBufferWritePos >= mOutputDelayRingBufferReadPos + numSamples)) {
 if (samples != 0) {
 for (int32_t i = 0; i < numSamples; i++) {
                samples[i] = mOutputDelayRingBuffer[mOutputDelayRingBufferReadPos++];
 }
 } else {
            mOutputDelayRingBufferReadPos += numSamples;
 }
 if (mOutputDelayRingBufferReadPos >= mOutputDelayRingBufferSize) {
            mOutputDelayRingBufferReadPos -= mOutputDelayRingBufferSize;
 }
 } else {
        ALOGV("slow SoftAAC2::outputDelayRingBufferGetSamples()");

 for (int32_t i = 0; i < numSamples; i++) {
 if (samples != 0) {
                samples[i] = mOutputDelayRingBuffer[mOutputDelayRingBufferReadPos];
 }
            mOutputDelayRingBufferReadPos++;
 if (mOutputDelayRingBufferReadPos >= mOutputDelayRingBufferSize) {
                mOutputDelayRingBufferReadPos -= mOutputDelayRingBufferSize;
 }
 }
 }
    mOutputDelayRingBufferFilled -= numSamples;
 return numSamples;
}
