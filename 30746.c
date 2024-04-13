CCITTFaxStream::~CCITTFaxStream() {
  delete str;
  gfree(refLine);
  gfree(codingLine);
}
