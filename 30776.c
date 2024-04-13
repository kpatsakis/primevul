static void expandRow(Guchar *srcBuf, Guchar *dstBuf, int srcWidth, int scaledWidth, int nComps)
{
  double xStep = (double)srcWidth/scaledWidth;
  double xSrc = 0.0;
  double xFrac, xInt;
  int p;

  for (int i = 0; i < nComps; i++)
    srcBuf[srcWidth*nComps + i] = srcBuf[(srcWidth-1)*nComps + i];

  for (int x = 0; x < scaledWidth; x++) {
    xFrac = modf(xSrc, &xInt);
    p = (int)xInt;
    for (int c = 0; c < nComps; c++) {
      dstBuf[nComps*x + c] = srcBuf[nComps*p + c]*(1.0 - xFrac) + srcBuf[nComps*(p+1) + c]*xFrac;
    }
    xSrc += xStep;
  }
}
