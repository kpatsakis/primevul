status_t MPEG4Source::parseSampleAuxiliaryInformationOffsets(
 off64_t offset, off64_t /* size */) {
    ALOGV("parseSampleAuxiliaryInformationOffsets");
 uint8_t version;
 if (mDataSource->readAt(offset, &version, sizeof(version)) != 1) {
 return ERROR_IO;
 }
    offset++;

 uint32_t flags;
 if (!mDataSource->getUInt24(offset, &flags)) {
 return ERROR_IO;
 }
    offset += 3;

 uint32_t entrycount;
 if (!mDataSource->getUInt32(offset, &entrycount)) {
 return ERROR_IO;
 }
    offset += 4;
 if (entrycount == 0) {
 return OK;
 }
 if (entrycount > UINT32_MAX / 8) {
 return ERROR_MALFORMED;
 }

 if (entrycount > mCurrentSampleInfoOffsetsAllocSize) {
 uint64_t *newPtr = (uint64_t *)realloc(mCurrentSampleInfoOffsets, entrycount * 8);
 if (newPtr == NULL) {
            ALOGE("failed to realloc %u -> %u", mCurrentSampleInfoOffsetsAllocSize, entrycount * 8);
 return NO_MEMORY;
 }
        mCurrentSampleInfoOffsets = newPtr;
        mCurrentSampleInfoOffsetsAllocSize = entrycount;
 }
    mCurrentSampleInfoOffsetCount = entrycount;

 if (mCurrentSampleInfoOffsets == NULL) {
 return OK;
 }

 for (size_t i = 0; i < entrycount; i++) {
 if (version == 0) {
 uint32_t tmp;
 if (!mDataSource->getUInt32(offset, &tmp)) {
 return ERROR_IO;
 }
            mCurrentSampleInfoOffsets[i] = tmp;
            offset += 4;
 } else {
 uint64_t tmp;
 if (!mDataSource->getUInt64(offset, &tmp)) {
 return ERROR_IO;
 }
            mCurrentSampleInfoOffsets[i] = tmp;
            offset += 8;
 }
 }


 off64_t drmoffset = mCurrentSampleInfoOffsets[0]; // from moof

    drmoffset += mCurrentMoofOffset;
 int ivlength;
    CHECK(mFormat->findInt32(kKeyCryptoDefaultIVSize, &ivlength));

 if (ivlength != 0 && ivlength != 8 && ivlength != 16) {
        ALOGW("unsupported IV length: %d", ivlength);
 return ERROR_MALFORMED;
 }
 for (size_t i = 0; i < mCurrentSampleInfoCount; i++) {
 if (i >= mCurrentSamples.size()) {
            ALOGW("too few samples");
 break;
 }
 Sample *smpl = &mCurrentSamples.editItemAt(i);

        memset(smpl->iv, 0, 16);
 if (mDataSource->readAt(drmoffset, smpl->iv, ivlength) != ivlength) {
 return ERROR_IO;
 }

        drmoffset += ivlength;

 int32_t smplinfosize = mCurrentDefaultSampleInfoSize;
 if (smplinfosize == 0) {
            smplinfosize = mCurrentSampleInfoSizes[i];
 }
 if (smplinfosize > ivlength) {
 uint16_t numsubsamples;
 if (!mDataSource->getUInt16(drmoffset, &numsubsamples)) {
 return ERROR_IO;
 }
            drmoffset += 2;
 for (size_t j = 0; j < numsubsamples; j++) {
 uint16_t numclear;
 uint32_t numencrypted;
 if (!mDataSource->getUInt16(drmoffset, &numclear)) {
 return ERROR_IO;
 }
                drmoffset += 2;
 if (!mDataSource->getUInt32(drmoffset, &numencrypted)) {
 return ERROR_IO;
 }
                drmoffset += 4;
                smpl->clearsizes.add(numclear);
                smpl->encryptedsizes.add(numencrypted);
 }
 } else {
            smpl->clearsizes.add(0);
            smpl->encryptedsizes.add(smpl->size);
 }
 }


 return OK;
}
