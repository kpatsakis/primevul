GBool DCTStream::readProgressiveDataUnit(DCTHuffTable *dcHuffTable,
					 DCTHuffTable *acHuffTable,
					 int *prevDC, int data[64]) {
  int run, size, amp, bit, c;
  int i, j, k;

  i = scanInfo.firstCoeff;
  if (i == 0) {
    if (scanInfo.ah == 0) {
      if ((size = readHuffSym(dcHuffTable)) == 9999) {
	return gFalse;
      }
      if (size > 0) {
	if ((amp = readAmp(size)) == 9999) {
	  return gFalse;
	}
      } else {
	amp = 0;
      }
      data[0] += (*prevDC += amp) << scanInfo.al;
    } else {
      if ((bit = readBit()) == 9999) {
	return gFalse;
      }
      data[0] += bit << scanInfo.al;
    }
    ++i;
  }
  if (scanInfo.lastCoeff == 0) {
    return gTrue;
  }

  if (eobRun > 0) {
    while (i <= scanInfo.lastCoeff) {
      j = dctZigZag[i++];
      if (data[j] != 0) {
	if ((bit = readBit()) == EOF) {
	  return gFalse;
	}
	if (bit) {
	  data[j] += 1 << scanInfo.al;
	}
      }
    }
    --eobRun;
    return gTrue;
  }

  while (i <= scanInfo.lastCoeff) {
    if ((c = readHuffSym(acHuffTable)) == 9999) {
      return gFalse;
    }

    if (c == 0xf0) {
      k = 0;
      while (k < 16 && i <= scanInfo.lastCoeff) {
	j = dctZigZag[i++];
	if (data[j] == 0) {
	  ++k;
	} else {
	  if ((bit = readBit()) == EOF) {
	    return gFalse;
	  }
	  if (bit) {
	    data[j] += 1 << scanInfo.al;
	  }
	}
      }

    } else if ((c & 0x0f) == 0x00) {
      j = c >> 4;
      eobRun = 0;
      for (k = 0; k < j; ++k) {
	if ((bit = readBit()) == EOF) {
	  return gFalse;
	}
	eobRun = (eobRun << 1) | bit;
      }
      eobRun += 1 << j;
      while (i <= scanInfo.lastCoeff) {
	j = dctZigZag[i++];
	if (data[j] != 0) {
	  if ((bit = readBit()) == EOF) {
	    return gFalse;
	  }
	  if (bit) {
	    data[j] += 1 << scanInfo.al;
	  }
	}
      }
      --eobRun;
      break;

    } else {
      run = (c >> 4) & 0x0f;
      size = c & 0x0f;
      if ((amp = readAmp(size)) == 9999) {
	return gFalse;
      }
      j = 0; // make gcc happy
      for (k = 0; k <= run && i <= scanInfo.lastCoeff; ++k) {
	j = dctZigZag[i++];
	while (data[j] != 0 && i <= scanInfo.lastCoeff) {
	  if ((bit = readBit()) == EOF) {
	    return gFalse;
	  }
	  if (bit) {
	    data[j] += 1 << scanInfo.al;
	  }
	  j = dctZigZag[i++];
	}
      }
      data[j] = amp << scanInfo.al;
    }
  }

  return gTrue;
}
