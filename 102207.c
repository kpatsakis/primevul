void ATSParser::signalDiscontinuity(
 DiscontinuityType type, const sp<AMessage> &extra) {
 int64_t mediaTimeUs;
 if ((type & DISCONTINUITY_TIME) && extra != NULL) {
 if (extra->findInt64(IStreamListener::kKeyMediaTimeUs, &mediaTimeUs)) {
            mAbsoluteTimeAnchorUs = mediaTimeUs;
 }
 if ((mFlags & TS_TIMESTAMPS_ARE_ABSOLUTE)
 && extra->findInt64(
 IStreamListener::kKeyRecentMediaTimeUs, &mediaTimeUs)) {
 if (mAbsoluteTimeAnchorUs >= 0ll) {
                mediaTimeUs -= mAbsoluteTimeAnchorUs;
 }
 if (mTimeOffsetValid) {
                mediaTimeUs -= mTimeOffsetUs;
 }
            mLastRecoveredPTS = (mediaTimeUs * 9) / 100;
 }
 } else if (type == DISCONTINUITY_ABSOLUTE_TIME) {
 int64_t timeUs;
 if (!extra->findInt64("timeUs", &timeUs)) {
            ALOGE("timeUs not found");
 return;
 }

 if (!mPrograms.empty()) {
            ALOGE("mPrograms is not empty");
 return;
 }
        mAbsoluteTimeAnchorUs = timeUs;
 return;
 } else if (type == DISCONTINUITY_TIME_OFFSET) {
 int64_t offset;
 if (!extra->findInt64("offset", &offset)) {
            ALOGE("offset not found");
 return;
 }

        mTimeOffsetValid = true;
        mTimeOffsetUs = offset;
 return;
 }

 for (size_t i = 0; i < mPrograms.size(); ++i) {
        mPrograms.editItemAt(i)->signalDiscontinuity(type, extra);
 }
}
