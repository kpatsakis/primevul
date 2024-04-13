void ImageStream::skipLine() {
  str->doGetChars(inputLineSize, inputLine);
}
