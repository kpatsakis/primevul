JBIG2Bitmap::JBIG2Bitmap(Guint segNumA, JBIG2Bitmap *bitmap):
  JBIG2Segment(segNumA)
{
  if (unlikely(bitmap == NULL)) {
    error(errSyntaxError, -1, "NULL bitmap in JBIG2Bitmap");
    w = h = line = 0;
    data = NULL;
    return;
  }

  w = bitmap->w;
  h = bitmap->h;
  line = bitmap->line;

  if (w <= 0 || h <= 0 || line <= 0 || h >= (INT_MAX - 1) / line) {
    error(errSyntaxError, -1, "invalid width/height");
    data = NULL;
    return;
  }
  data = (Guchar *)gmalloc(h * line + 1);
  memcpy(data, bitmap->data, h * line);
  data[h * line] = 0;
}
