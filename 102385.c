bool SkIcoCodec::onDimensionsSupported(const SkISize& dim) {
 return this->chooseCodec(dim, 0) >= 0;
}
