int DCTStream::getChar() {
  int c;

  if (y >= height) {
    return EOF;
  }
  if (progressive || !interleaved) {
    c = frameBuf[comp][y * bufWidth + x];
    if (++comp == numComps) {
      comp = 0;
      if (++x == width) {
	x = 0;
	++y;
      }
    }
  } else {
    if (dy >= mcuHeight) {
      if (!readMCURow()) {
	y = height;
	return EOF;
      }
      comp = 0;
      x = 0;
      dy = 0;
    }
    c = rowBuf[comp][dy][x];
    if (++comp == numComps) {
      comp = 0;
      if (++x == width) {
	x = 0;
	++y;
	++dy;
	if (y == height) {
	  readTrailer();
	}
      }
    }
  }
  return c;
}
