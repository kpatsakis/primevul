FixedLengthEncoder::~FixedLengthEncoder() {
  if (str->isEncoder())
    delete str;
}
