GBool FlateStream::startBlock() {
  int blockHdr;
  int c;
  int check;

  if (litCodeTab.codes != fixedLitCodeTab.codes) {
    gfree(litCodeTab.codes);
  }
  litCodeTab.codes = NULL;
  if (distCodeTab.codes != fixedDistCodeTab.codes) {
    gfree(distCodeTab.codes);
  }
  distCodeTab.codes = NULL;

  blockHdr = getCodeWord(3);
  if (blockHdr & 1)
    eof = gTrue;
  blockHdr >>= 1;

  if (blockHdr == 0) {
    compressedBlock = gFalse;
    if ((c = str->getChar()) == EOF)
      goto err;
    blockLen = c & 0xff;
    if ((c = str->getChar()) == EOF)
      goto err;
    blockLen |= (c & 0xff) << 8;
    if ((c = str->getChar()) == EOF)
      goto err;
    check = c & 0xff;
    if ((c = str->getChar()) == EOF)
      goto err;
    check |= (c & 0xff) << 8;
    if (check != (~blockLen & 0xffff))
      error(errSyntaxError, getPos(), "Bad uncompressed block length in flate stream");
    codeBuf = 0;
    codeSize = 0;

  } else if (blockHdr == 1) {
    compressedBlock = gTrue;
    loadFixedCodes();

  } else if (blockHdr == 2) {
    compressedBlock = gTrue;
    if (!readDynamicCodes()) {
      goto err;
    }

  } else {
    goto err;
  }

  endOfBlock = gFalse;
  return gTrue;

err:
  error(errSyntaxError, getPos(), "Bad block header in flate stream");
  endOfBlock = eof = gTrue;
  return gFalse;
}
