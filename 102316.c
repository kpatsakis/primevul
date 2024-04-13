MPEG4Source::MPEG4Source(
 const sp<MPEG4Extractor> &owner,
 const sp<MetaData> &format,
 const sp<DataSource> &dataSource,
 int32_t timeScale,
 const sp<SampleTable> &sampleTable,
 Vector<SidxEntry> &sidx,
 const Trex *trex,
 off64_t firstMoofOffset)
 : mOwner(owner),
      mFormat(format),
      mDataSource(dataSource),
      mTimescale(timeScale),
      mSampleTable(sampleTable),
      mCurrentSampleIndex(0),
      mCurrentFragmentIndex(0),
      mSegments(sidx),
      mTrex(trex),
      mFirstMoofOffset(firstMoofOffset),
      mCurrentMoofOffset(firstMoofOffset),
      mNextMoofOffset(-1),
      mCurrentTime(0),
      mCurrentSampleInfoAllocSize(0),
      mCurrentSampleInfoSizes(NULL),
      mCurrentSampleInfoOffsetsAllocSize(0),
      mCurrentSampleInfoOffsets(NULL),
      mIsAVC(false),
      mIsHEVC(false),
      mNALLengthSize(0),
      mStarted(false),
      mGroup(NULL),
      mBuffer(NULL),
      mWantsNALFragments(false),
      mSrcBuffer(NULL) {

    memset(&mTrackFragmentHeaderInfo, 0, sizeof(mTrackFragmentHeaderInfo));

    mFormat->findInt32(kKeyCryptoMode, &mCryptoMode);
    mDefaultIVSize = 0;
    mFormat->findInt32(kKeyCryptoDefaultIVSize, &mDefaultIVSize);
 uint32_t keytype;
 const void *key;
 size_t keysize;
 if (mFormat->findData(kKeyCryptoKey, &keytype, &key, &keysize)) {
        CHECK(keysize <= 16);
        memset(mCryptoKey, 0, 16);
        memcpy(mCryptoKey, key, keysize);
 }

 const char *mime;
 bool success = mFormat->findCString(kKeyMIMEType, &mime);
    CHECK(success);

    mIsAVC = !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC);
    mIsHEVC = !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_HEVC);

 if (mIsAVC) {
 uint32_t type;
 const void *data;
 size_t size;
        CHECK(format->findData(kKeyAVCC, &type, &data, &size));

 const uint8_t *ptr = (const uint8_t *)data;

        CHECK(size >= 7);
        CHECK_EQ((unsigned)ptr[0], 1u); // configurationVersion == 1

        mNALLengthSize = 1 + (ptr[4] & 3);
 } else if (mIsHEVC) {
 uint32_t type;
 const void *data;
 size_t size;
        CHECK(format->findData(kKeyHVCC, &type, &data, &size));

 const uint8_t *ptr = (const uint8_t *)data;

        CHECK(size >= 22);
        CHECK_EQ((unsigned)ptr[0], 1u); // configurationVersion == 1

        mNALLengthSize = 1 + (ptr[14 + 7] & 3);
 }

    CHECK(format->findInt32(kKeyTrackID, &mTrackId));

}
