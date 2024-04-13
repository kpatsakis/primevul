status_t MediaPlayerService::AudioOutput::open(
 uint32_t sampleRate, int channelCount, audio_channel_mask_t channelMask,
 audio_format_t format, int bufferCount,
 AudioCallback cb, void *cookie,
 audio_output_flags_t flags,
 const audio_offload_info_t *offloadInfo,
 bool doNotReconnect,
 uint32_t suggestedFrameCount)
{
    ALOGV("open(%u, %d, 0x%x, 0x%x, %d, %d 0x%x)", sampleRate, channelCount, channelMask,
                format, bufferCount, mSessionId, flags);

 if (((flags & AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD) != 0) &&
 ((cb == NULL) || (offloadInfo == NULL))) {
 return BAD_VALUE;
 }

 size_t frameCount;
 if ((flags & AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD) != 0) {
        frameCount = 0; // AudioTrack will get frame count from AudioFlinger
 } else {
 uint32_t afSampleRate;
 size_t afFrameCount;
 if (AudioSystem::getOutputFrameCount(&afFrameCount, mStreamType) != NO_ERROR) {
 return NO_INIT;
 }
 if (AudioSystem::getOutputSamplingRate(&afSampleRate, mStreamType) != NO_ERROR) {
 return NO_INIT;
 }
 const size_t framesPerBuffer =
 (unsigned long long)sampleRate * afFrameCount / afSampleRate;

 if (bufferCount == 0) {
            bufferCount = (suggestedFrameCount + framesPerBuffer - 1) / framesPerBuffer;
 }
 if (bufferCount != 0 && bufferCount < mMinBufferCount) {
            ALOGV("bufferCount (%d) increased to %d", bufferCount, mMinBufferCount);
            bufferCount = mMinBufferCount;
 }
        frameCount = bufferCount * framesPerBuffer;
 }

 if (channelMask == CHANNEL_MASK_USE_CHANNEL_ORDER) {
        channelMask = audio_channel_out_mask_from_count(channelCount);
 if (0 == channelMask) {
            ALOGE("open() error, can\'t derive mask for %d audio channels", channelCount);
 return NO_INIT;
 }
 }

 Mutex::Autolock lock(mLock);
    mCallback = cb;
    mCallbackCookie = cookie;

 bool reuse = false;
 bool bothOffloaded = false;

 if (mRecycledTrack != 0) {
        bothOffloaded = (flags & mRecycledTrack->getFlags()
 & AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD) != 0;

        reuse = true;

 if ((mCallbackData == NULL && mCallback != NULL) ||
 (mCallbackData != NULL && mCallback == NULL)) {
            ALOGV("can't chain callback and write");
            reuse = false;
 } else if ((mRecycledTrack->getSampleRate() != sampleRate) ||
 (mRecycledTrack->channelCount() != (uint32_t)channelCount) ) {
            ALOGV("samplerate, channelcount differ: %u/%u Hz, %u/%d ch",
                  mRecycledTrack->getSampleRate(), sampleRate,
                  mRecycledTrack->channelCount(), channelCount);
            reuse = false;
 } else if (flags != mFlags) {
            ALOGV("output flags differ %08x/%08x", flags, mFlags);
            reuse = false;
 } else if (mRecycledTrack->format() != format) {
            reuse = false;
 }
 } else {
        ALOGV("no track available to recycle");
 }

    ALOGV_IF(bothOffloaded, "both tracks offloaded");

 if (bothOffloaded && !reuse) {
        ALOGV("both offloaded and not recycling");
        deleteRecycledTrack_l();
 }

    sp<AudioTrack> t;
 CallbackData *newcbd = NULL;


 if (!(reuse && bothOffloaded)) {
        ALOGV("creating new AudioTrack");

 if (mCallback != NULL) {
            newcbd = new CallbackData(this);
            t = new AudioTrack(
                    mStreamType,
                    sampleRate,
                    format,
                    channelMask,
                    frameCount,
                    flags,
 CallbackWrapper,
                    newcbd,
 0, // notification frames
                    mSessionId,
 AudioTrack::TRANSFER_CALLBACK,
                    offloadInfo,
                    mUid,
                    mPid,
                    mAttributes,
                    doNotReconnect);
 } else {
 const float targetSpeed =
                    std::min(std::max(mPlaybackRate.mSpeed, 1.0f), kMaxRequiredSpeed);
            ALOGW_IF(targetSpeed != mPlaybackRate.mSpeed,
 "track target speed:%f clamped from playback speed:%f",
                    targetSpeed, mPlaybackRate.mSpeed);
            t = new AudioTrack(
                    mStreamType,
                    sampleRate,
                    format,
                    channelMask,
                    frameCount,
                    flags,
                    NULL, // callback
                    NULL, // user data
 0, // notification frames
                    mSessionId,
 AudioTrack::TRANSFER_DEFAULT,
                    NULL, // offload info
                    mUid,
                    mPid,
                    mAttributes,
                    doNotReconnect,
                    targetSpeed);
 }

 if ((t == 0) || (t->initCheck() != NO_ERROR)) {
            ALOGE("Unable to create audio track");
 delete newcbd;
 return NO_INIT;
 } else {
            mStreamType = t->streamType();
 }
 }

 if (reuse) {
        CHECK(mRecycledTrack != NULL);

 if (!bothOffloaded) {
 if (mRecycledTrack->frameCount() != t->frameCount()) {
                ALOGV("framecount differs: %zu/%zu frames",
                      mRecycledTrack->frameCount(), t->frameCount());
                reuse = false;
 }
 }

 if (reuse) {
            ALOGV("chaining to next output and recycling track");
            close_l();
            mTrack = mRecycledTrack;
            mRecycledTrack.clear();
 if (mCallbackData != NULL) {
                mCallbackData->setOutput(this);
 }
 delete newcbd;
 return OK;
 }
 }

 if (!bothOffloaded) {
        deleteRecycledTrack_l();
 }

    CHECK((t != NULL) && ((mCallback == NULL) || (newcbd != NULL)));

    mCallbackData = newcbd;
    ALOGV("setVolume");
    t->setVolume(mLeftVolume, mRightVolume);

    mSampleRateHz = sampleRate;
    mFlags = flags;
    mMsecsPerFrame = 1E3f / (mPlaybackRate.mSpeed * sampleRate);
    mFrameSize = t->frameSize();
    mTrack = t;

 status_t res = NO_ERROR;
 if ((t->getFlags() & (AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD | AUDIO_OUTPUT_FLAG_DIRECT)) == 0) {
        res = t->setPlaybackRate(mPlaybackRate);
 if (res == NO_ERROR) {
            t->setAuxEffectSendLevel(mSendLevel);
            res = t->attachAuxEffect(mAuxEffectId);
 }
 }
    ALOGV("open() DONE status %d", res);
 return res;
}
