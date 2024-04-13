status_t ATSParser::Stream::parse(
 unsigned continuity_counter,
 unsigned payload_unit_start_indicator, ABitReader *br,
 SyncEvent *event) {
 if (mQueue == NULL) {
 return OK;
 }

 if (mExpectedContinuityCounter >= 0
 && (unsigned)mExpectedContinuityCounter != continuity_counter) {
        ALOGI("discontinuity on stream pid 0x%04x", mElementaryPID);

        mPayloadStarted = false;
        mBuffer->setRange(0, 0);
        mExpectedContinuityCounter = -1;

#if 0

 if (mStreamType == STREAMTYPE_H264) {
            ALOGI("clearing video queue");
            mQueue->clear(true /* clearFormat */);
 }
#endif

 if (!payload_unit_start_indicator) {
 return OK;
 }
 }

    mExpectedContinuityCounter = (continuity_counter + 1) & 0x0f;

 if (payload_unit_start_indicator) {
 off64_t offset = (event != NULL) ? event->getOffset() : 0;
 if (mPayloadStarted) {

 status_t err = flush(event);

 if (err != OK) {
                ALOGW("Error (%08x) happened while flushing; we simply discard "
 "the PES packet and continue.", err);
 }
 }

        mPayloadStarted = true;
        mPesStartOffset = offset;
 }

 if (!mPayloadStarted) {
 return OK;
 }

 size_t payloadSizeBits = br->numBitsLeft();
 if (payloadSizeBits % 8 != 0u) {
        ALOGE("Wrong value");
 return BAD_VALUE;
 }

 size_t neededSize = mBuffer->size() + payloadSizeBits / 8;
 if (mBuffer->capacity() < neededSize) {
        neededSize = (neededSize + 65535) & ~65535;

        ALOGI("resizing buffer to %zu bytes", neededSize);

        sp<ABuffer> newBuffer = new ABuffer(neededSize);
        memcpy(newBuffer->data(), mBuffer->data(), mBuffer->size());
        newBuffer->setRange(0, mBuffer->size());
        mBuffer = newBuffer;
 }

    memcpy(mBuffer->data() + mBuffer->size(), br->data(), payloadSizeBits / 8);
    mBuffer->setRange(0, mBuffer->size() + payloadSizeBits / 8);

 return OK;
}
