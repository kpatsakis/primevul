DCTStream::DCTStream(Stream *strA, int colorXformA):
    FilterStream(strA) {
  int i, j;

  colorXform = colorXformA;
  progressive = interleaved = gFalse;
  width = height = 0;
  mcuWidth = mcuHeight = 0;
  numComps = 0;
  comp = 0;
  x = y = dy = 0;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 32; ++j) {
      rowBuf[i][j] = NULL;
    }
    frameBuf[i] = NULL;
  }

  if (!dctClipInit) {
    for (i = -256; i < 0; ++i)
      dctClip[dctClipOffset + i] = 0;
    for (i = 0; i < 256; ++i)
      dctClip[dctClipOffset + i] = i;
    for (i = 256; i < 512; ++i)
      dctClip[dctClipOffset + i] = 255;
    dctClipInit = 1;
  }
}
