void JBIG2Stream::readPatternDictSeg(Guint segNum, Guint length) {
  JBIG2PatternDict *patternDict;
  JBIG2Bitmap *bitmap;
  Guint flags, patternW, patternH, grayMax, templ, mmr;
  int atx[4], aty[4];
  Guint i, x;

  if (!readUByte(&flags) ||
      !readUByte(&patternW) ||
      !readUByte(&patternH) ||
      !readULong(&grayMax)) {
    goto eofError;
  }
  templ = (flags >> 1) & 3;
  mmr = flags & 1;

  if (!mmr) {
    resetGenericStats(templ, NULL);
    arithDecoder->start();
  }

  atx[0] = -(int)patternW; aty[0] =  0;
  atx[1] = -3;             aty[1] = -1;
  atx[2] =  2;             aty[2] = -2;
  atx[3] = -2;             aty[3] = -2;
  bitmap = readGenericBitmap(mmr, (grayMax + 1) * patternW, patternH,
			     templ, gFalse, gFalse, NULL,
			     atx, aty, length - 7);

  if (!bitmap)
    return;

  patternDict = new JBIG2PatternDict(segNum, grayMax + 1);

  x = 0;
  for (i = 0; i <= grayMax && i < patternDict->getSize(); ++i) {
    patternDict->setBitmap(i, bitmap->getSlice(x, 0, patternW, patternH));
    x += patternW;
  }

  delete bitmap;

  segments->append(patternDict);

  return;

 eofError:
  error(errSyntaxError, curStr->getPos(), "Unexpected EOF in JBIG2 stream");
}
