GBool StreamPredictor::getNextLine() {
  int curPred;
  Guchar upLeftBuf[gfxColorMaxComps * 2 + 1];
  int left, up, upLeft, p, pa, pb, pc;
  int c;
  Gulong inBuf, outBuf, bitMask;
  int inBits, outBits;
  int i, j, k, kk;

  if (predictor >= 10) {
    if ((curPred = str->getRawChar()) == EOF) {
      return gFalse;
    }
    curPred += 10;
  } else {
    curPred = predictor;
  }

  int *rawCharLine = new int[rowBytes - pixBytes];
  str->getRawChars(rowBytes - pixBytes, rawCharLine);
  memset(upLeftBuf, 0, pixBytes + 1);
  for (i = pixBytes; i < rowBytes; ++i) {
    for (j = pixBytes; j > 0; --j) {
      upLeftBuf[j] = upLeftBuf[j-1];
    }
    upLeftBuf[0] = predLine[i];
    if ((c = rawCharLine[i - pixBytes]) == EOF) {
      if (i > pixBytes) {
	break;
      }
      delete[] rawCharLine;
      return gFalse;
    }
    switch (curPred) {
    case 11:			// PNG sub
      predLine[i] = predLine[i - pixBytes] + (Guchar)c;
      break;
    case 12:			// PNG up
      predLine[i] = predLine[i] + (Guchar)c;
      break;
    case 13:			// PNG average
      predLine[i] = ((predLine[i - pixBytes] + predLine[i]) >> 1) +
	            (Guchar)c;
      break;
    case 14:			// PNG Paeth
      left = predLine[i - pixBytes];
      up = predLine[i];
      upLeft = upLeftBuf[pixBytes];
      p = left + up - upLeft;
      if ((pa = p - left) < 0)
	pa = -pa;
      if ((pb = p - up) < 0)
	pb = -pb;
      if ((pc = p - upLeft) < 0)
	pc = -pc;
      if (pa <= pb && pa <= pc)
	predLine[i] = left + (Guchar)c;
      else if (pb <= pc)
	predLine[i] = up + (Guchar)c;
      else
	predLine[i] = upLeft + (Guchar)c;
      break;
    case 10:			// PNG none
    default:			// no predictor or TIFF predictor
      predLine[i] = (Guchar)c;
      break;
    }
  }
  delete[] rawCharLine;

  if (predictor == 2) {
    if (nBits == 1) {
      inBuf = predLine[pixBytes - 1];
      for (i = pixBytes; i < rowBytes; i += 8) {
	inBuf = (inBuf << 8) | predLine[i];
	predLine[i] ^= inBuf >> nComps;
      }
    } else if (nBits == 8) {
      for (i = pixBytes; i < rowBytes; ++i) {
	predLine[i] += predLine[i - nComps];
      }
    } else {
      memset(upLeftBuf, 0, nComps + 1);
      bitMask = (1 << nBits) - 1;
      inBuf = outBuf = 0;
      inBits = outBits = 0;
      j = k = pixBytes;
      for (i = 0; i < width; ++i) {
	for (kk = 0; kk < nComps; ++kk) {
	  if (inBits < nBits) {
	    inBuf = (inBuf << 8) | (predLine[j++] & 0xff);
	    inBits += 8;
	  }
	  upLeftBuf[kk] = (Guchar)((upLeftBuf[kk] +
				    (inBuf >> (inBits - nBits))) & bitMask);
	  inBits -= nBits;
	  outBuf = (outBuf << nBits) | upLeftBuf[kk];
	  outBits += nBits;
	  if (outBits >= 8) {
	    predLine[k++] = (Guchar)(outBuf >> (outBits - 8));
	    outBits -= 8;
	  }
	}
      }
      if (outBits > 0) {
	predLine[k++] = (Guchar)((outBuf << (8 - outBits)) +
				 (inBuf & ((1 << (8 - outBits)) - 1)));
      }
    }
  }

  predIdx = pixBytes;

  return gTrue;
}
