static inline String Decode16BitUnicodeEscapeSequences(const String& string) {
  return DecodeEscapeSequences<Unicode16BitEscapeSequence>(string,
                                                           UTF8Encoding());
}
