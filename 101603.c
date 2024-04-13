CString TextCodecUTF8::Encode(const LChar* characters,
                              wtf_size_t length,
                              UnencodableHandling) {
  return EncodeCommon(characters, length);
}
