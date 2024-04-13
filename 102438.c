void ACodec::dumpBuffers(OMX_U32 portIndex) {
    CHECK(portIndex == kPortIndexInput || portIndex == kPortIndexOutput);
    ALOGI("[%s] %s port has %zu buffers:", mComponentName.c_str(),
            portIndex == kPortIndexInput ? "input" : "output", mBuffers[portIndex].size());
 for (size_t i = 0; i < mBuffers[portIndex].size(); ++i) {
 const BufferInfo &info = mBuffers[portIndex][i];
        ALOGI("  slot %2zu: #%8u %p/%p %s(%d) dequeued:%u",
                i, info.mBufferID, info.mGraphicBuffer.get(),
                info.mGraphicBuffer == NULL ? NULL : info.mGraphicBuffer->getNativeBuffer(),
                _asString(info.mStatus), info.mStatus, info.mDequeuedAt);
 }
}
