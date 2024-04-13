Guchar *ImageStream::getLine() {
  Gulong buf, bitMask;
  int bits;
  int c;
  int i;
  Guchar *p;
 
  int readChars = str->doGetChars(inputLineSize, inputLine);
  for ( ; readChars < inputLineSize; readChars++) inputLine[readChars] = EOF;
  if (nBits == 1) {
    p = inputLine;
    for (i = 0; i < nVals; i += 8) {
      c = *p++;
      imgLine[i+0] = (Guchar)((c >> 7) & 1);
      imgLine[i+1] = (Guchar)((c >> 6) & 1);
      imgLine[i+2] = (Guchar)((c >> 5) & 1);
      imgLine[i+3] = (Guchar)((c >> 4) & 1);
      imgLine[i+4] = (Guchar)((c >> 3) & 1);
      imgLine[i+5] = (Guchar)((c >> 2) & 1);
      imgLine[i+6] = (Guchar)((c >> 1) & 1);
      imgLine[i+7] = (Guchar)(c & 1);
    }
  } else if (nBits == 8) {
  } else if (nBits == 16) {
    p = inputLine;
    for (i = 0; i < nVals; ++i) {
      imgLine[i] = *p++;
      p++;
    }
  } else {
    bitMask = (1 << nBits) - 1;
    buf = 0;
    bits = 0;
    p = inputLine;
    for (i = 0; i < nVals; ++i) {
      if (bits < nBits) {
	buf = (buf << 8) | (*p++ & 0xff);
	bits += 8;
      }
      imgLine[i] = (Guchar)((buf >> (bits - nBits)) & bitMask);
      bits -= nBits;
    }
  }
  return imgLine;
}
