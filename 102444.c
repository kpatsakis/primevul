bool ACodec::BaseState::onOMXEvent(
        OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2) {
 if (event != OMX_EventError) {
        ALOGV("[%s] EVENT(%d, 0x%08x, 0x%08x)",
             mCodec->mComponentName.c_str(), event, data1, data2);

 return false;
 }

    ALOGE("[%s] ERROR(0x%08x)", mCodec->mComponentName.c_str(), data1);

    OMX_ERRORTYPE omxError = (OMX_ERRORTYPE)data1;
 if (!isOMXError(omxError)) {
        ALOGW("Invalid OMX error %#x", omxError);
        omxError = OMX_ErrorUndefined;
 }
    mCodec->signalError(omxError);

 return true;
}
