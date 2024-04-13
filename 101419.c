 const unsigned char* TestBlinkPlatformSupport::getTraceCategoryEnabledFlag(
    const char* categoryName) {
  static const unsigned char tracingIsDisabled = 0;
  return &tracingIsDisabled;
}
