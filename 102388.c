bool SkIcoCodec::onSkipScanlines(int count) {
 SkASSERT(fCurrScanlineCodec);
 return fCurrScanlineCodec->skipScanlines(count);
}
