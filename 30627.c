void DCTStream::close() {
  int i, j;

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 32; ++j) {
      gfree(rowBuf[i][j]);
      rowBuf[i][j] = NULL;
    }
    gfree(frameBuf[i]);
    frameBuf[i] = NULL;
  }
  FilterStream::close();
}
