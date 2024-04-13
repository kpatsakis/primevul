status_t OMXCodec::allocateOutputBuffersFromNativeWindow() {
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
 if (err != OK) {
        CODEC_LOGE("getParameter failed: %d", err);
 return err;
 }

    sp<MetaData> meta = mSource->getFormat();

 int32_t rotationDegrees;
 if (!meta->findInt32(kKeyRotation, &rotationDegrees)) {
        rotationDegrees = 0;
 }

    OMX_U32 usage = 0;
    err = mOMX->getGraphicBufferUsage(mNode, kPortIndexOutput, &usage);
 if (err != 0) {
        ALOGW("querying usage flags from OMX IL component failed: %d", err);
        usage = 0;
 }

 if (mFlags & kEnableGrallocUsageProtected) {
        usage |= GRALLOC_USAGE_PROTECTED;
 }

    err = setNativeWindowSizeFormatAndUsage(
            mNativeWindow.get(),
            def.format.video.nFrameWidth,
            def.format.video.nFrameHeight,
            def.format.video.eColorFormat,
            rotationDegrees,
            usage | GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_EXTERNAL_DISP);
 if (err != 0) {
 return err;
 }

 int minUndequeuedBufs = 0;
    err = mNativeWindow->query(mNativeWindow.get(),
            NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS, &minUndequeuedBufs);
 if (err != 0) {
        ALOGE("NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS query failed: %s (%d)",
                strerror(-err), -err);
 return err;
 }

    CODEC_LOGI("OMX-buffers: min=%u actual=%u undeq=%d+1",
            def.nBufferCountMin, def.nBufferCountActual, minUndequeuedBufs);

 for (OMX_U32 extraBuffers = 2 + 1; /* condition inside loop */; extraBuffers--) {
        OMX_U32 newBufferCount =
            def.nBufferCountMin + minUndequeuedBufs + extraBuffers;
        def.nBufferCountActual = newBufferCount;
        err = mOMX->setParameter(
                mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err == OK) {
            minUndequeuedBufs += extraBuffers;
 break;
 }

        CODEC_LOGW("setting nBufferCountActual to %u failed: %d",
                newBufferCount, err);
 /* exit condition */
 if (extraBuffers == 0) {
 return err;
 }
 }
    CODEC_LOGI("OMX-buffers: min=%u actual=%u undeq=%d+1",
            def.nBufferCountMin, def.nBufferCountActual, minUndequeuedBufs);

    err = native_window_set_buffer_count(
            mNativeWindow.get(), def.nBufferCountActual);
 if (err != 0) {
        ALOGE("native_window_set_buffer_count failed: %s (%d)", strerror(-err),
 -err);
 return err;
 }

    CODEC_LOGV("allocating %u buffers from a native window of size %u on "
 "output port", def.nBufferCountActual, def.nBufferSize);

 for (OMX_U32 i = 0; i < def.nBufferCountActual; i++) {
 ANativeWindowBuffer* buf;
        err = native_window_dequeue_buffer_and_wait(mNativeWindow.get(), &buf);
 if (err != 0) {
            ALOGE("dequeueBuffer failed: %s (%d)", strerror(-err), -err);
 break;
 }

        sp<GraphicBuffer> graphicBuffer(new GraphicBuffer(buf, false));
 BufferInfo info;
        info.mData = NULL;
        info.mSize = def.nBufferSize;
        info.mStatus = OWNED_BY_US;
        info.mMem = NULL;
        info.mMediaBuffer = new MediaBuffer(graphicBuffer);
        info.mMediaBuffer->setObserver(this);
        mPortBuffers[kPortIndexOutput].push(info);

        IOMX::buffer_id bufferId;
        err = mOMX->useGraphicBuffer(mNode, kPortIndexOutput, graphicBuffer,
 &bufferId);
 if (err != 0) {
            CODEC_LOGE("registering GraphicBuffer with OMX IL component "
 "failed: %d", err);
 break;
 }

        mPortBuffers[kPortIndexOutput].editItemAt(i).mBuffer = bufferId;

        CODEC_LOGV("registered graphic buffer with ID %u (pointer = %p)",
                bufferId, graphicBuffer.get());
 }

    OMX_U32 cancelStart;
    OMX_U32 cancelEnd;
 if (err != 0) {
        cancelStart = 0;
        cancelEnd = mPortBuffers[kPortIndexOutput].size();
 } else {
        cancelStart = def.nBufferCountActual - minUndequeuedBufs;
        cancelEnd = def.nBufferCountActual;
 }

 for (OMX_U32 i = cancelStart; i < cancelEnd; i++) {
 BufferInfo *info = &mPortBuffers[kPortIndexOutput].editItemAt(i);
        cancelBufferToNativeWindow(info);
 }

 return err;
}
