OMX_ERRORTYPE SimpleSoftOMXComponent::emptyThisBuffer(
        OMX_BUFFERHEADERTYPE *buffer) {
    sp<AMessage> msg = new AMessage(kWhatEmptyThisBuffer, mHandler);
    msg->setPointer("header", buffer);
    msg->post();

 return OMX_ErrorNone;
}
