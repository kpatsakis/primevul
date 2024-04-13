void CLASS nokia_load_raw()
{
  uchar  *data,  *dp;
  int rev, dwide, row, col, c;

  rev = 3 * (order == 0x4949);
  dwide = (raw_width * 5 + 1) / 4;
  data = (uchar *) malloc (dwide*2);
  merror (data, "nokia_load_raw()");
#ifdef LIBRAW_LIBRARY_BUILD
  try {
#endif
  for (row=0; row < raw_height; row++) {
#ifdef LIBRAW_LIBRARY_BUILD
    checkCancel();
#endif
    if (fread (data+dwide, 1, dwide, ifp) < dwide) derror();
    FORC(dwide) data[c] = data[dwide+(c ^ rev)];
    for (dp=data, col=0; col < raw_width; dp+=5, col+=4)
      FORC4 RAW(row,col+c) = (dp[c] << 2) | (dp[4] >> (c << 1) & 3);
  }
#ifdef LIBRAW_LIBRARY_BUILD
  } catch (...){
    free (data);
    throw;
  }
#endif
  free (data);
  maximum = 0x3ff;
}