bool OMXCodec::findCodecQuirks(const char *componentName, uint32_t *quirks) {
 const sp<IMediaCodecList> list = MediaCodecList::getInstance();
 if (list == NULL) {
 return false;
 }

 ssize_t index = list->findCodecByName(componentName);

 if (index < 0) {
 return false;
 }

 const sp<MediaCodecInfo> info = list->getCodecInfo(index);
    CHECK(info != NULL);
 *quirks = getComponentQuirks(info);

 return true;
}
