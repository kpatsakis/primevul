void ATSParser::updatePCR(
 unsigned /* PID */, uint64_t PCR, size_t byteOffsetFromStart) {
    ALOGV("PCR 0x%016" PRIx64 " @ %zu", PCR, byteOffsetFromStart);

 if (mNumPCRs == 2) {
        mPCR[0] = mPCR[1];
        mPCRBytes[0] = mPCRBytes[1];
        mSystemTimeUs[0] = mSystemTimeUs[1];
        mNumPCRs = 1;
 }

    mPCR[mNumPCRs] = PCR;
    mPCRBytes[mNumPCRs] = byteOffsetFromStart;
    mSystemTimeUs[mNumPCRs] = ALooper::GetNowUs();

 ++mNumPCRs;

 if (mNumPCRs == 2) {
 double transportRate =
 (mPCRBytes[1] - mPCRBytes[0]) * 27E6 / (mPCR[1] - mPCR[0]);

        ALOGV("transportRate = %.2f bytes/sec", transportRate);
 }
}
