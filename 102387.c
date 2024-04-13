SkCodec::SkScanlineOrder SkIcoCodec::onGetScanlineOrder() const {
 if (fCurrScanlineCodec) {
 SkASSERT(!fCurrIncrementalCodec);
 return fCurrScanlineCodec->getScanlineOrder();
 }

 if (fCurrIncrementalCodec) {
 return fCurrIncrementalCodec->getScanlineOrder();
 }

 return INHERITED::onGetScanlineOrder();
}
