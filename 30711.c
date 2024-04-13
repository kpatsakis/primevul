void FlateStream::readSome() {
  int code1, code2;
  int len, dist;
  int i, j, k;
  int c;

  if (endOfBlock) {
    if (!startBlock())
      return;
  }

  if (compressedBlock) {
    if ((code1 = getHuffmanCodeWord(&litCodeTab)) == EOF)
      goto err;
    if (code1 < 256) {
      buf[index] = code1;
      remain = 1;
    } else if (code1 == 256) {
      endOfBlock = gTrue;
      remain = 0;
    } else {
      code1 -= 257;
      code2 = lengthDecode[code1].bits;
      if (code2 > 0 && (code2 = getCodeWord(code2)) == EOF)
	goto err;
      len = lengthDecode[code1].first + code2;
      if ((code1 = getHuffmanCodeWord(&distCodeTab)) == EOF)
	goto err;
      code2 = distDecode[code1].bits;
      if (code2 > 0 && (code2 = getCodeWord(code2)) == EOF)
	goto err;
      dist = distDecode[code1].first + code2;
      i = index;
      j = (index - dist) & flateMask;
      for (k = 0; k < len; ++k) {
	buf[i] = buf[j];
	i = (i + 1) & flateMask;
	j = (j + 1) & flateMask;
      }
      remain = len;
    }

  } else {
    len = (blockLen < flateWindow) ? blockLen : flateWindow;
    for (i = 0, j = index; i < len; ++i, j = (j + 1) & flateMask) {
      if ((c = str->getChar()) == EOF) {
	endOfBlock = eof = gTrue;
	break;
      }
      buf[j] = c & 0xff;
    }
    remain = i;
    blockLen -= len;
    if (blockLen == 0)
      endOfBlock = gTrue;
  }

  return;

err:
  error(errSyntaxError, getPos(), "Unexpected end of file in flate stream");
  endOfBlock = eof = gTrue;
  remain = 0;
}
