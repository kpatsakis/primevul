RunLengthEncoder::~RunLengthEncoder() {
  if (str->isEncoder())
    delete str;
}
