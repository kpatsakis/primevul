status_t BufferQueueConsumer::disconnect() {
    ATRACE_CALL();

    BQ_LOGV("disconnect(C)");

 Mutex::Autolock lock(mCore->mMutex);

 if (mCore->mConsumerListener == NULL) {
        BQ_LOGE("disconnect(C): no consumer is connected");
 return BAD_VALUE;
 }

    mCore->mIsAbandoned = true;
    mCore->mConsumerListener = NULL;
    mCore->mQueue.clear();
    mCore->freeAllBuffersLocked();
    mCore->mDequeueCondition.broadcast();
 return NO_ERROR;
}
