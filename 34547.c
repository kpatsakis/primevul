JBIG2SymbolDict::JBIG2SymbolDict(Guint segNumA, Guint sizeA):
  JBIG2Segment(segNumA)
{
  Guint i;

  size = sizeA;
  bitmaps = (JBIG2Bitmap **)gmallocn_checkoverflow(size, sizeof(JBIG2Bitmap *));
  if (!bitmaps) size = 0;
  for (i = 0; i < size; ++i) {
    bitmaps[i] = NULL;
  }
  genericRegionStats = NULL;
  refinementRegionStats = NULL;
}
