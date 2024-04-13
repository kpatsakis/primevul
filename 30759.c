RGBGrayEncoder::~RGBGrayEncoder() {
  if (str->isEncoder())
    delete str;
}
