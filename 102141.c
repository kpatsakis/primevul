void OMXCodec::findMatchingCodecs(
 const char *mime,
 bool createEncoder, const char *matchComponentName,
 uint32_t flags,
 Vector<CodecNameAndQuirks> *matchingCodecs) {
    matchingCodecs->clear();

 const sp<IMediaCodecList> list = MediaCodecList::getInstance();
 if (list == NULL) {
 return;
 }

 size_t index = 0;
 for (;;) {
 ssize_t matchIndex =
 list->findCodecByType(mime, createEncoder, index);

 if (matchIndex < 0) {
 break;
 }

        index = matchIndex + 1;

 const sp<MediaCodecInfo> info = list->getCodecInfo(matchIndex);
        CHECK(info != NULL);
 const char *componentName = info->getCodecName();

 if (matchComponentName && strcmp(componentName, matchComponentName)) {
 continue;
 }

 if (((flags & kSoftwareCodecsOnly) && IsSoftwareCodec(componentName)) ||
 ((flags & kHardwareCodecsOnly) && !IsSoftwareCodec(componentName)) ||
 (!(flags & (kSoftwareCodecsOnly | kHardwareCodecsOnly)))) {

 ssize_t index = matchingCodecs->add();
 CodecNameAndQuirks *entry = &matchingCodecs->editItemAt(index);
            entry->mName = String8(componentName);
            entry->mQuirks = getComponentQuirks(info);

            ALOGV("matching '%s' quirks 0x%08x",
                  entry->mName.string(), entry->mQuirks);
 }
 }

 if (flags & kPreferSoftwareCodecs) {
        matchingCodecs->sort(CompareSoftwareCodecsFirst);
 }
}
