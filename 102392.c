media_status_t AMediaCodec_releaseOutputBuffer(AMediaCodec *mData, size_t idx, bool render) {
 if (render) {
 return translate_error(mData->mCodec->renderOutputBufferAndRelease(idx));
 } else {
 return translate_error(mData->mCodec->releaseOutputBuffer(idx));
 }
}
