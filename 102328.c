status_t SampleTable::setTimeToSampleParams(
 off64_t data_offset, size_t data_size) {
 if (mHasTimeToSample || data_size < 8) {
 return ERROR_MALFORMED;
 }

 uint8_t header[8];
 if (mDataSource->readAt(
                data_offset, header, sizeof(header)) < (ssize_t)sizeof(header)) {
 return ERROR_IO;
 }

 if (U32_AT(header) != 0) {
 return ERROR_MALFORMED;
 }

    mTimeToSampleCount = U32_AT(&header[4]);
 if (mTimeToSampleCount > UINT32_MAX / (2 * sizeof(uint32_t))) {
        ALOGE("Time-to-sample table size too large.");
 return ERROR_OUT_OF_RANGE;
 }


 uint64_t allocSize = (uint64_t)mTimeToSampleCount * 2 * sizeof(uint32_t);
    mTotalSize += allocSize;
 if (mTotalSize > kMaxTotalSize) {
        ALOGE("Time-to-sample table size would make sample table too large.\n"
 "    Requested time-to-sample table size = %llu\n"
 "    Eventual sample table size >= %llu\n"
 "    Allowed sample table size = %llu\n",
 (unsigned long long)allocSize,
 (unsigned long long)mTotalSize,
 (unsigned long long)kMaxTotalSize);
 return ERROR_OUT_OF_RANGE;
 }

    mTimeToSample = new (std::nothrow) uint32_t[mTimeToSampleCount * 2];
 if (!mTimeToSample) {
        ALOGE("Cannot allocate time-to-sample table with %llu entries.",
 (unsigned long long)mTimeToSampleCount);
 return ERROR_OUT_OF_RANGE;
 }

 if (mDataSource->readAt(data_offset + 8, mTimeToSample,
 (size_t)allocSize) < (ssize_t)allocSize) {
        ALOGE("Incomplete data read for time-to-sample table.");
 return ERROR_IO;
 }

 for (size_t i = 0; i < mTimeToSampleCount * 2; ++i) {
        mTimeToSample[i] = ntohl(mTimeToSample[i]);
 }

    mHasTimeToSample = true;
 return OK;
}
