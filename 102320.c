status_t MyOggExtractor::_readNextPacket(MediaBuffer **out, bool calcVorbisTimestamp) {
 *out = NULL;

 MediaBuffer *buffer = NULL;
 int64_t timeUs = -1;

 for (;;) {
 size_t i;
 size_t packetSize = 0;
 bool gotFullPacket = false;
 for (i = mNextLaceIndex; i < mCurrentPage.mNumSegments; ++i) {
 uint8_t lace = mCurrentPage.mLace[i];

            packetSize += lace;

 if (lace < 255) {
                gotFullPacket = true;
 ++i;
 break;
 }
 }

 if (mNextLaceIndex < mCurrentPage.mNumSegments) {
 off64_t dataOffset = mOffset + 27 + mCurrentPage.mNumSegments;
 for (size_t j = 0; j < mNextLaceIndex; ++j) {
                dataOffset += mCurrentPage.mLace[j];
 }

 size_t fullSize = packetSize;
 if (buffer != NULL) {
                fullSize += buffer->range_length();
 }
 if (fullSize > 16 * 1024 * 1024) { // arbitrary limit of 16 MB packet size
 if (buffer != NULL) {
                    buffer->release();
 }
                ALOGE("b/36592202");
 return ERROR_MALFORMED;
 }
 MediaBuffer *tmp = new (std::nothrow) MediaBuffer(fullSize);
 if (tmp == NULL) {
 if (buffer != NULL) {
                    buffer->release();
 }
                ALOGE("b/36592202");
 return ERROR_MALFORMED;
 }
 if (buffer != NULL) {
                memcpy(tmp->data(), buffer->data(), buffer->range_length());
                tmp->set_range(0, buffer->range_length());
                buffer->release();
 } else {
                tmp->set_range(0, 0);
 }
            buffer = tmp;

 ssize_t n = mSource->readAt(
                    dataOffset,
 (uint8_t *)buffer->data() + buffer->range_length(),
                    packetSize);

 if (n < (ssize_t)packetSize) {
                buffer->release();
                ALOGV("failed to read %zu bytes at %#016llx, got %zd bytes",
                        packetSize, (long long)dataOffset, n);
 return ERROR_IO;
 }

            buffer->set_range(0, fullSize);

            mNextLaceIndex = i;

 if (gotFullPacket) {

 if (mFirstPacketInPage) {
                    buffer->meta_data()->setInt32(
                            kKeyValidSamples, mCurrentPageSamples);
                    mFirstPacketInPage = false;
 }

 if (calcVorbisTimestamp) {
 int32_t curBlockSize = getPacketBlockSize(buffer);
 if (mCurrentPage.mPrevPacketSize < 0) {
                        mCurrentPage.mPrevPacketSize = curBlockSize;
                        mCurrentPage.mPrevPacketPos =
                                mCurrentPage.mGranulePosition - mCurrentPageSamples;
                        timeUs = mCurrentPage.mPrevPacketPos * 1000000ll / mVi.rate;
 } else {
 int32_t actualBlockSize =
 (curBlockSize + mCurrentPage.mPrevPacketSize) / 2;
                        timeUs = mCurrentPage.mPrevPacketPos * 1000000ll / mVi.rate;
                        mCurrentPage.mPrevPacketPos += actualBlockSize / 2;
                        mCurrentPage.mPrevPacketSize = curBlockSize;
 }
                    buffer->meta_data()->setInt64(kKeyTime, timeUs);
 }
 *out = buffer;

 return OK;
 }

 }

        CHECK_EQ(mNextLaceIndex, mCurrentPage.mNumSegments);

        mOffset += mCurrentPageSize;
 ssize_t n = readPage(mOffset, &mCurrentPage);

 if (n <= 0) {
 if (buffer) {
                buffer->release();
                buffer = NULL;
 }

            ALOGV("readPage returned %zd", n);

 return n < 0 ? n : (status_t)ERROR_END_OF_STREAM;
 }

 if (mCurrentPage.mGranulePosition >= mPrevGranulePosition) {
            mCurrentPageSamples =
                    mCurrentPage.mGranulePosition - mPrevGranulePosition;
 } else {
            mCurrentPageSamples = 0;
 }
        mFirstPacketInPage = true;

        mPrevGranulePosition = mCurrentPage.mGranulePosition;

        mCurrentPageSize = n;
        mNextLaceIndex = 0;

 if (buffer != NULL) {
 if ((mCurrentPage.mFlags & 1) == 0) {

 if (timeUs >= 0) {
                    buffer->meta_data()->setInt64(kKeyTime, timeUs);
 }

                buffer->meta_data()->setInt32(
                        kKeyValidSamples, mCurrentPageSamples);
                mFirstPacketInPage = false;

 *out = buffer;

 return OK;
 }
 }
 }
}
