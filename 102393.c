media_status_t AMediaCodec_setOutputSurface(AMediaCodec *mData, ANativeWindow* window) {
    sp<Surface> surface = NULL;
 if (window != NULL) {
        surface = (Surface*) window;
 }
 return translate_error(mData->mCodec->setSurface(surface));
}
