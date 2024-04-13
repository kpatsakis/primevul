JBIG2PatternDict::JBIG2PatternDict(Guint segNumA, Guint sizeA):
  JBIG2Segment(segNumA)
{
  bitmaps = (JBIG2Bitmap **)gmallocn_checkoverflow(sizeA, sizeof(JBIG2Bitmap *));
  if (bitmaps) {
    size = sizeA;
  } else {
    size = 0;
    error(errSyntaxError, -1, "JBIG2PatternDict: can't allocate bitmaps");
  }
}
