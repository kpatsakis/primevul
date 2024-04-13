media_status_t AMediaCodec_configure(
 AMediaCodec *mData,
 const AMediaFormat* format,
 ANativeWindow* window,
 AMediaCrypto *crypto,
 uint32_t flags) {
    sp<AMessage> nativeFormat;
 AMediaFormat_getFormat(format, &nativeFormat);
    ALOGV("configure with format: %s", nativeFormat->debugString(0).c_str());
    sp<Surface> surface = NULL;
 if (window != NULL) {
        surface = (Surface*) window;
 }

 status_t err = mData->mCodec->configure(nativeFormat, surface,
            crypto ? crypto->mCrypto : NULL, flags);
 if (err != OK) {
        ALOGE("configure: err(%d), failed with format: %s",
              err, nativeFormat->debugString(0).c_str());
 }
 return translate_error(err);
}
