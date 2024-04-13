void Splash::vertFlipImage(SplashBitmap *img, int width, int height,
			   int nComps) {
  Guchar *lineBuf;
  Guchar *p0, *p1;
  int w;

  w = width * nComps;
  lineBuf = (Guchar *)gmalloc(w);
  for (p0 = img->data, p1 = img->data + (height - 1) * w;
       p0 < p1;
       p0 += w, p1 -= w) {
    memcpy(lineBuf, p0, w);
    memcpy(p0, p1, w);
    memcpy(p1, lineBuf, w);
  }
  if (img->alpha) {
    for (p0 = img->alpha, p1 = img->alpha + (height - 1) * width;
	 p0 < p1;
	 p0 += width, p1 -= width) {
      memcpy(lineBuf, p0, width);
      memcpy(p0, p1, width);
      memcpy(p1, lineBuf, width);
    }
  }
  gfree(lineBuf);
}
