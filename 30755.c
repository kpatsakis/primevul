ImageStream::~ImageStream() {
  if (imgLine != (Guchar *)inputLine) {
    gfree(imgLine);
  }
  gfree(inputLine);
}
