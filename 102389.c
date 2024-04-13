SkCodec::Result SkIcoCodec::onStartIncrementalDecode(const SkImageInfo& dstInfo,
 void* pixels, size_t rowBytes, const SkCodec::Options& options) {
 int index = 0;
 while (true) {
        index = this->chooseCodec(dstInfo.dimensions(), index);
 if (index < 0) {
 break;
 }

 SkCodec* embeddedCodec = fEmbeddedCodecs->operator[](index).get();
 switch (embeddedCodec->startIncrementalDecode(dstInfo,
                pixels, rowBytes, &options)) {
 case kSuccess:
                fCurrIncrementalCodec = embeddedCodec;
                fCurrScanlineCodec = nullptr;
 return kSuccess;
 case kUnimplemented:
 if (embeddedCodec->startScanlineDecode(dstInfo) == kSuccess) {
 return kUnimplemented;
 }
 break;
 default:
 break;
 }

        index++;
 }

 SkCodecPrintf("Error: No matching candidate image in ico.\n");
 return kInvalidScale;
}
