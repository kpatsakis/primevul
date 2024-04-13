void SampleTable::buildSampleEntriesTable() {
 Mutex::Autolock autoLock(mLock);

 if (mSampleTimeEntries != NULL || mNumSampleSizes == 0) {
 if (mNumSampleSizes == 0) {
            ALOGE("b/23247055, mNumSampleSizes(%u)", mNumSampleSizes);
 }
 return;
 }

    mTotalSize += (uint64_t)mNumSampleSizes * sizeof(SampleTimeEntry);
 if (mTotalSize > kMaxTotalSize) {
        ALOGE("Sample entry table size would make sample table too large.\n"
 "    Requested sample entry table size = %llu\n"
 "    Eventual sample table size >= %llu\n"
 "    Allowed sample table size = %llu\n",
 (unsigned long long)mNumSampleSizes * sizeof(SampleTimeEntry),
 (unsigned long long)mTotalSize,
 (unsigned long long)kMaxTotalSize);
 return;
 }

    mSampleTimeEntries = new (std::nothrow) SampleTimeEntry[mNumSampleSizes];
 if (!mSampleTimeEntries) {
        ALOGE("Cannot allocate sample entry table with %llu entries.",
 (unsigned long long)mNumSampleSizes);
 return;
 }

 uint32_t sampleIndex = 0;
 uint32_t sampleTime = 0;

 for (uint32_t i = 0; i < mTimeToSampleCount; ++i) {
 uint32_t n = mTimeToSample[2 * i];
 uint32_t delta = mTimeToSample[2 * i + 1];

 for (uint32_t j = 0; j < n; ++j) {
 if (sampleIndex < mNumSampleSizes) {

                mSampleTimeEntries[sampleIndex].mSampleIndex = sampleIndex;

 int32_t compTimeDelta =
                    mCompositionDeltaLookup->getCompositionTimeOffset(
                            sampleIndex);

 if ((compTimeDelta < 0 && sampleTime <
 (compTimeDelta == INT32_MIN ?
                                INT32_MAX : uint32_t(-compTimeDelta)))
 || (compTimeDelta > 0 &&
                                sampleTime > UINT32_MAX - compTimeDelta)) {
                    ALOGE("%u + %d would overflow, clamping",
                            sampleTime, compTimeDelta);
 if (compTimeDelta < 0) {
                        sampleTime = 0;
 } else {
                        sampleTime = UINT32_MAX;
 }
                    compTimeDelta = 0;
 }

                mSampleTimeEntries[sampleIndex].mCompositionTime =
                        compTimeDelta > 0 ? sampleTime + compTimeDelta:
                                sampleTime - (-compTimeDelta);
 }

 ++sampleIndex;
            sampleTime += delta;
 }
 }

    qsort(mSampleTimeEntries, mNumSampleSizes, sizeof(SampleTimeEntry),
 CompareIncreasingTime);
}
