bool ACodec::allYourBuffersAreBelongToUs(
        OMX_U32 portIndex) {
 for (size_t i = 0; i < mBuffers[portIndex].size(); ++i) {
 BufferInfo *info = &mBuffers[portIndex].editItemAt(i);

 if (info->mStatus != BufferInfo::OWNED_BY_US
 && info->mStatus != BufferInfo::OWNED_BY_NATIVE_WINDOW) {
            ALOGV("[%s] Buffer %u on port %u still has status %d",
                    mComponentName.c_str(),
                    info->mBufferID, portIndex, info->mStatus);
 return false;
 }
 }

 return true;
}
