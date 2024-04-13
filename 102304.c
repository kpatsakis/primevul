status_t NuPlayer::GenericSource::initFromDataSource() {
    sp<MediaExtractor> extractor;

    CHECK(mDataSource != NULL);

 if (mIsWidevine) {
 String8 mimeType;
 float confidence;
        sp<AMessage> dummy;
 bool success;

        success = SniffWVM(mDataSource, &mimeType, &confidence, &dummy);
 if (!success
 || strcasecmp(
                    mimeType.string(), MEDIA_MIMETYPE_CONTAINER_WVM)) {
            ALOGE("unsupported widevine mime: %s", mimeType.string());
 return UNKNOWN_ERROR;
 }

        mWVMExtractor = new WVMExtractor(mDataSource);
        mWVMExtractor->setAdaptiveStreamingMode(true);
 if (mUIDValid) {
            mWVMExtractor->setUID(mUID);
 }
        extractor = mWVMExtractor;
 } else {
        extractor = MediaExtractor::Create(mDataSource,
                mSniffedMIME.empty() ? NULL: mSniffedMIME.c_str());
 }

 if (extractor == NULL) {
 return UNKNOWN_ERROR;
 }

 if (extractor->getDrmFlag()) {
        checkDrmStatus(mDataSource);
 }

    mFileMeta = extractor->getMetaData();
 if (mFileMeta != NULL) {
 int64_t duration;
 if (mFileMeta->findInt64(kKeyDuration, &duration)) {
            mDurationUs = duration;
 }
 }

 int32_t totalBitrate = 0;

 size_t numtracks = extractor->countTracks();
 if (numtracks == 0) {
 return UNKNOWN_ERROR;
 }

 for (size_t i = 0; i < numtracks; ++i) {
        sp<MediaSource> track = extractor->getTrack(i);

        sp<MetaData> meta = extractor->getTrackMetaData(i);

 const char *mime;
        CHECK(meta->findCString(kKeyMIMEType, &mime));

 if (!strncasecmp(mime, "audio/", 6)) {
 if (mAudioTrack.mSource == NULL) {
                mAudioTrack.mIndex = i;
                mAudioTrack.mSource = track;
                mAudioTrack.mPackets =
 new AnotherPacketSource(mAudioTrack.mSource->getFormat());

 if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_VORBIS)) {
                    mAudioIsVorbis = true;
 } else {
                    mAudioIsVorbis = false;
 }
 }
 } else if (!strncasecmp(mime, "video/", 6)) {
 if (mVideoTrack.mSource == NULL) {
                mVideoTrack.mIndex = i;
                mVideoTrack.mSource = track;
                mVideoTrack.mPackets =
 new AnotherPacketSource(mVideoTrack.mSource->getFormat());

 int32_t secure;
 if (meta->findInt32(kKeyRequiresSecureBuffers, &secure)
 && secure) {
                    mIsWidevine = true;
 if (mUIDValid) {
                        extractor->setUID(mUID);
 }
 }
 }
 }

 if (track != NULL) {
            mSources.push(track);
 int64_t durationUs;
 if (meta->findInt64(kKeyDuration, &durationUs)) {
 if (durationUs > mDurationUs) {
                    mDurationUs = durationUs;
 }
 }

 int32_t bitrate;
 if (totalBitrate >= 0 && meta->findInt32(kKeyBitRate, &bitrate)) {
                totalBitrate += bitrate;
 } else {
                totalBitrate = -1;
 }
 }
 }

    mBitrate = totalBitrate;

 return OK;
}
