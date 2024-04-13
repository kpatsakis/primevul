status_t MPEG4Extractor::parseSegmentIndex(off64_t offset, size_t size) {
  ALOGV("MPEG4Extractor::parseSegmentIndex");

 if (size < 12) {
 return -EINVAL;
 }

 uint32_t flags;
 if (!mDataSource->getUInt32(offset, &flags)) {
 return ERROR_MALFORMED;
 }

 uint32_t version = flags >> 24;
    flags &= 0xffffff;

    ALOGV("sidx version %d", version);

 uint32_t referenceId;
 if (!mDataSource->getUInt32(offset + 4, &referenceId)) {
 return ERROR_MALFORMED;
 }

 uint32_t timeScale;
 if (!mDataSource->getUInt32(offset + 8, &timeScale)) {
 return ERROR_MALFORMED;
 }
    ALOGV("sidx refid/timescale: %d/%d", referenceId, timeScale);

 uint64_t earliestPresentationTime;
 uint64_t firstOffset;

    offset += 12;
    size -= 12;

 if (version == 0) {
 if (size < 8) {
 return -EINVAL;
 }
 uint32_t tmp;
 if (!mDataSource->getUInt32(offset, &tmp)) {
 return ERROR_MALFORMED;
 }
        earliestPresentationTime = tmp;
 if (!mDataSource->getUInt32(offset + 4, &tmp)) {
 return ERROR_MALFORMED;
 }
        firstOffset = tmp;
        offset += 8;
        size -= 8;
 } else {
 if (size < 16) {
 return -EINVAL;
 }
 if (!mDataSource->getUInt64(offset, &earliestPresentationTime)) {
 return ERROR_MALFORMED;
 }
 if (!mDataSource->getUInt64(offset + 8, &firstOffset)) {
 return ERROR_MALFORMED;
 }
        offset += 16;
        size -= 16;
 }
    ALOGV("sidx pres/off: %Ld/%Ld", earliestPresentationTime, firstOffset);

 if (size < 4) {
 return -EINVAL;
 }

 uint16_t referenceCount;
 if (!mDataSource->getUInt16(offset + 2, &referenceCount)) {
 return ERROR_MALFORMED;
 }
    offset += 4;
    size -= 4;
    ALOGV("refcount: %d", referenceCount);

 if (size < referenceCount * 12) {
 return -EINVAL;
 }

 uint64_t total_duration = 0;
 for (unsigned int i = 0; i < referenceCount; i++) {
 uint32_t d1, d2, d3;

 if (!mDataSource->getUInt32(offset, &d1) || // size
 !mDataSource->getUInt32(offset + 4, &d2) || // duration
 !mDataSource->getUInt32(offset + 8, &d3)) { // flags
 return ERROR_MALFORMED;
 }

 if (d1 & 0x80000000) {
            ALOGW("sub-sidx boxes not supported yet");
 }
 bool sap = d3 & 0x80000000;
 bool saptype = d3 >> 28;
 if (!sap || saptype > 2) {
            ALOGW("not a stream access point, or unsupported type");
 }
        total_duration += d2;
        offset += 12;
        ALOGV(" item %d, %08x %08x %08x", i, d1, d2, d3);
 SidxEntry se;
        se.mSize = d1 & 0x7fffffff;
        se.mDurationUs = 1000000LL * d2 / timeScale;
        mSidxEntries.add(se);
 }

    mSidxDuration = total_duration * 1000000 / timeScale;
    ALOGV("duration: %lld", mSidxDuration);

 int64_t metaDuration;
 if (!mLastTrack->meta->findInt64(kKeyDuration, &metaDuration) || metaDuration == 0) {
        mLastTrack->meta->setInt64(kKeyDuration, mSidxDuration);
 }
 return OK;
}
