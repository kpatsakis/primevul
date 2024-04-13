void FlateStream::loadFixedCodes() {
  litCodeTab.codes = fixedLitCodeTab.codes;
  litCodeTab.maxLen = fixedLitCodeTab.maxLen;
  distCodeTab.codes = fixedDistCodeTab.codes;
  distCodeTab.maxLen = fixedDistCodeTab.maxLen;
}
