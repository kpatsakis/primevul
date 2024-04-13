OMX_ERRORTYPE SimpleSoftOMXComponent::freeBuffer(
        OMX_U32 portIndex,
        OMX_BUFFERHEADERTYPE *header) {
 Mutex::Autolock autoLock(mLock);

    CHECK_LT(portIndex, mPorts.size());

 PortInfo *port = &mPorts.editItemAt(portIndex);

#if 0 // XXX
    CHECK((mState == OMX_StateIdle && mTargetState == OMX_StateLoaded)
 || port->mDef.bEnabled == OMX_FALSE);
#endif

 bool found = false;
 for (size_t i = 0; i < port->mBuffers.size(); ++i) {
 BufferInfo *buffer = &port->mBuffers.editItemAt(i);

 if (buffer->mHeader == header) {
            CHECK(!buffer->mOwnedByUs);

 if (header->pPlatformPrivate != NULL) {
                CHECK(header->pPlatformPrivate == header->pBuffer);

 delete[] header->pBuffer;
                header->pBuffer = NULL;
 }

 delete header;
            header = NULL;

            port->mBuffers.removeAt(i);
            port->mDef.bPopulated = OMX_FALSE;

            checkTransitions();

            found = true;
 break;
 }
 }

    CHECK(found);

 return OMX_ErrorNone;
}
