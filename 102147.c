status_t OMXCodec::stopOmxComponent_l() {
    CODEC_LOGV("stopOmxComponent_l mState=%d", mState);

 while (isIntermediateState(mState)) {
        mAsyncCompletion.wait(mLock);
 }

 bool isError = false;
 switch (mState) {
 case LOADED:
 break;

 case ERROR:
 {
 if (mPortStatus[kPortIndexOutput] == ENABLING) {
                freeBuffersOnPort(kPortIndexInput, true /* onlyThoseWeOwn */);
                freeBuffersOnPort(kPortIndexOutput, true /* onlyThoseWeOwn */);
                setState(LOADED);
 break;
 } else {
                OMX_STATETYPE state = OMX_StateInvalid;
 status_t err = mOMX->getState(mNode, &state);
                CHECK_EQ(err, (status_t)OK);

 if (state != OMX_StateExecuting) {
 break;
 }
 }

            isError = true;
 }

 case EXECUTING:
 {
            setState(EXECUTING_TO_IDLE);

 if (mQuirks & kRequiresFlushBeforeShutdown) {
                CODEC_LOGV("This component requires a flush before transitioning "
 "from EXECUTING to IDLE...");

 bool emulateInputFlushCompletion =
 !flushPortAsync(kPortIndexInput);

 bool emulateOutputFlushCompletion =
 !flushPortAsync(kPortIndexOutput);

 if (emulateInputFlushCompletion) {
                    onCmdComplete(OMX_CommandFlush, kPortIndexInput);
 }

 if (emulateOutputFlushCompletion) {
                    onCmdComplete(OMX_CommandFlush, kPortIndexOutput);
 }
 } else {
                mPortStatus[kPortIndexInput] = SHUTTING_DOWN;
                mPortStatus[kPortIndexOutput] = SHUTTING_DOWN;

 status_t err =
                    mOMX->sendCommand(mNode, OMX_CommandStateSet, OMX_StateIdle);
                CHECK_EQ(err, (status_t)OK);
 }

 while (mState != LOADED && mState != ERROR) {
                mAsyncCompletion.wait(mLock);
 }

 if (isError) {
                setState(ERROR);
 }

 break;
 }

 default:
 {
            CHECK(!"should not be here.");
 break;
 }
 }

 if (mLeftOverBuffer) {
        mLeftOverBuffer->release();
        mLeftOverBuffer = NULL;
 }

 return OK;
}
