status_t BufferQueueConsumer::detachBuffer(int slot) {
    ATRACE_CALL();
    ATRACE_BUFFER_INDEX(slot);
    BQ_LOGV("detachBuffer(C): slot %d", slot);
 Mutex::Autolock lock(mCore->mMutex);

 if (mCore->mIsAbandoned) {
        BQ_LOGE("detachBuffer(C): BufferQueue has been abandoned");
 return NO_INIT;
 }

 if (slot < 0 || slot >= BufferQueueDefs::NUM_BUFFER_SLOTS) {
        BQ_LOGE("detachBuffer(C): slot index %d out of range [0, %d)",
                slot, BufferQueueDefs::NUM_BUFFER_SLOTS);
 return BAD_VALUE;
 } else if (mSlots[slot].mBufferState != BufferSlot::ACQUIRED) {
        BQ_LOGE("detachBuffer(C): slot %d is not owned by the consumer "
 "(state = %d)", slot, mSlots[slot].mBufferState);
 return BAD_VALUE;
 }

    mCore->freeBufferLocked(slot);
    mCore->mDequeueCondition.broadcast();

 return NO_ERROR;
}
