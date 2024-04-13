JBIG2Bitmap *JBIG2Stream::readGenericBitmap(GBool mmr, int w, int h,
					    int templ, GBool tpgdOn,
					    GBool useSkip, JBIG2Bitmap *skip,
					    int *atx, int *aty,
					    int mmrDataLength) {
  JBIG2Bitmap *bitmap;
  GBool ltp;
  Guint ltpCX, cx, cx0, cx1, cx2;
  int *refLine, *codingLine;
  int code1, code2, code3;
  Guchar *p0, *p1, *p2, *pp;
  Guchar *atP0, *atP1, *atP2, *atP3;
  Guint buf0, buf1, buf2;
  Guint atBuf0, atBuf1, atBuf2, atBuf3;
  int atShift0, atShift1, atShift2, atShift3;
  Guchar mask;
  int x, y, x0, x1, a0i, b1i, blackPixels, pix, i;

  bitmap = new JBIG2Bitmap(0, w, h);
  if (!bitmap->isOk()) {
    delete bitmap;
    return NULL;
  }
  bitmap->clearToZero();


  if (mmr) {

    mmrDecoder->reset();
    if (w > INT_MAX - 2) {
      error(errSyntaxError, curStr->getPos(), "Bad width in JBIG2 generic bitmap");
      w = -3;
    }
    codingLine = (int *)gmallocn(w + 1, sizeof(int));
    refLine = (int *)gmallocn(w + 2, sizeof(int));
    memset(refLine, 0, (w + 2) * sizeof(int));
    for (i = 0; i < w + 1; ++i) codingLine[i] = w;

    for (y = 0; y < h; ++y) {

      for (i = 0; codingLine[i] < w; ++i) {
	refLine[i] = codingLine[i];
      }
      refLine[i++] = w;
      refLine[i] = w;

      codingLine[0] = 0;
      a0i = 0;
      b1i = 0;
      blackPixels = 0;
      while (codingLine[a0i] < w) {
	code1 = mmrDecoder->get2DCode();
	switch (code1) {
	case twoDimPass:
          if (unlikely(b1i + 1 >= w + 2)) {
            break;
          }
          mmrAddPixels(refLine[b1i + 1], blackPixels, codingLine, &a0i, w);
          if (refLine[b1i + 1] < w) {
            b1i += 2;
          }
          break;
	case twoDimHoriz:
          code1 = code2 = 0;
          if (blackPixels) {
            do {
              code1 += code3 = mmrDecoder->getBlackCode();
            } while (code3 >= 64);
            do {
              code2 += code3 = mmrDecoder->getWhiteCode();
            } while (code3 >= 64);
          } else {
            do {
              code1 += code3 = mmrDecoder->getWhiteCode();
            } while (code3 >= 64);
            do {
              code2 += code3 = mmrDecoder->getBlackCode();
            } while (code3 >= 64);
          }
          mmrAddPixels(codingLine[a0i] + code1, blackPixels,
		       codingLine, &a0i, w);
          if (codingLine[a0i] < w) {
            mmrAddPixels(codingLine[a0i] + code2, blackPixels ^ 1,
			 codingLine, &a0i, w);
          }
          while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
            b1i += 2;
          }
          break;
	case twoDimVertR3:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixels(refLine[b1i] + 3, blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            ++b1i;
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case twoDimVertR2:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixels(refLine[b1i] + 2, blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            ++b1i;
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case twoDimVertR1:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixels(refLine[b1i] + 1, blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            ++b1i;
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case twoDimVert0:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixels(refLine[b1i], blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            ++b1i;
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case twoDimVertL3:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixelsNeg(refLine[b1i] - 3, blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            if (b1i > 0) {
              --b1i;
            } else {
              ++b1i;
            }
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case twoDimVertL2:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixelsNeg(refLine[b1i] - 2, blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            if (b1i > 0) {
              --b1i;
            } else {
              ++b1i;
            }
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case twoDimVertL1:
          if (unlikely(b1i >= w + 2)) {
            break;
          }
          mmrAddPixelsNeg(refLine[b1i] - 1, blackPixels, codingLine, &a0i, w);
          blackPixels ^= 1;
          if (codingLine[a0i] < w) {
            if (b1i > 0) {
              --b1i;
            } else {
              ++b1i;
            }
            while (likely(b1i < w + 2) && refLine[b1i] <= codingLine[a0i] && refLine[b1i] < w) {
              b1i += 2;
            }
          }
          break;
	case EOF:
          mmrAddPixels(w, 0, codingLine, &a0i, w);
          break;
	default:
	  error(errSyntaxError, curStr->getPos(), "Illegal code in JBIG2 MMR bitmap data");
          mmrAddPixels(w, 0, codingLine, &a0i, w);
	  break;
	}
      }

      i = 0;
      while (1) {
	for (x = codingLine[i]; x < codingLine[i+1]; ++x) {
	  bitmap->setPixel(x, y);
	}
	if (codingLine[i+1] >= w || codingLine[i+2] >= w) {
	  break;
	}
	i += 2;
      }
    }

    if (mmrDataLength >= 0) {
      mmrDecoder->skipTo(mmrDataLength);
    } else {
      if (mmrDecoder->get24Bits() != 0x001001) {
	error(errSyntaxError, curStr->getPos(), "Missing EOFB in JBIG2 MMR bitmap data");
      }
    }

    gfree(refLine);
    gfree(codingLine);


  } else {
    ltpCX = 0; // make gcc happy
    if (tpgdOn) {
      switch (templ) {
      case 0:
	ltpCX = 0x3953; // 001 11001 0101 0011
	break;
      case 1:
	ltpCX = 0x079a; // 0011 11001 101 0
	break;
      case 2:
	ltpCX = 0x0e3; // 001 1100 01 1
	break;
      case 3:
	ltpCX = 0x18a; // 01100 0101 1
	break;
      }
    }

    ltp = 0;
    cx = cx0 = cx1 = cx2 = 0; // make gcc happy
    for (y = 0; y < h; ++y) {

      if (tpgdOn) {
	if (arithDecoder->decodeBit(ltpCX, genericRegionStats)) {
	  ltp = !ltp;
	}
	if (ltp) {
	  if (y > 0) {
	    bitmap->duplicateRow(y, y-1);
	  }
	  continue;
	}
      }

      switch (templ) {
      case 0:

	p2 = pp = bitmap->getDataPtr() + y * bitmap->getLineSize();
	buf2 = *p2++ << 8;
	if (y >= 1) {
	  p1 = bitmap->getDataPtr() + (y - 1) * bitmap->getLineSize();
	  buf1 = *p1++ << 8;
	  if (y >= 2) {
	    p0 = bitmap->getDataPtr() + (y - 2) * bitmap->getLineSize();
	    buf0 = *p0++ << 8;
	  } else {
	    p0 = NULL;
	    buf0 = 0;
	  }
	} else {
	  p1 = p0 = NULL;
	  buf1 = buf0 = 0;
	}

	if (atx[0] >= -8 && atx[0] <= 8 &&
	    atx[1] >= -8 && atx[1] <= 8 &&
	    atx[2] >= -8 && atx[2] <= 8 &&
	    atx[3] >= -8 && atx[3] <= 8) {
	  if (y + aty[0] >= 0 && y + aty[0] < bitmap->getHeight()) {
	    atP0 = bitmap->getDataPtr() + (y + aty[0]) * bitmap->getLineSize();
	    atBuf0 = *atP0++ << 8;
	  } else {
	    atP0 = NULL;
	    atBuf0 = 0;
	  }
	  atShift0 = 15 - atx[0];
	  if (y + aty[1] >= 0 && y + aty[1] < bitmap->getHeight()) {
	    atP1 = bitmap->getDataPtr() + (y + aty[1]) * bitmap->getLineSize();
	    atBuf1 = *atP1++ << 8;
	  } else {
	    atP1 = NULL;
	    atBuf1 = 0;
	  }
	  atShift1 = 15 - atx[1];
	  if (y + aty[2] >= 0 && y + aty[2] < bitmap->getHeight()) {
	    atP2 = bitmap->getDataPtr() + (y + aty[2]) * bitmap->getLineSize();
	    atBuf2 = *atP2++ << 8;
	  } else {
	    atP2 = NULL;
	    atBuf2 = 0;
	  }
	  atShift2 = 15 - atx[2];
	  if (y + aty[3] >= 0 && y + aty[3] < bitmap->getHeight()) {
	    atP3 = bitmap->getDataPtr() + (y + aty[3]) * bitmap->getLineSize();
	    atBuf3 = *atP3++ << 8;
	  } else {
	    atP3 = NULL;
	    atBuf3 = 0;
	  }
	  atShift3 = 15 - atx[3];

	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p0) {
		buf0 |= *p0++;
	      }
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	      if (atP0) {
		atBuf0 |= *atP0++;
	      }
	      if (atP1) {
		atBuf1 |= *atP1++;
	      }
	      if (atP2) {
		atBuf2 |= *atP2++;
	      }
	      if (atP3) {
		atBuf3 |= *atP3++;
	      }
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx0 = (buf0 >> 14) & 0x07;
	      cx1 = (buf1 >> 13) & 0x1f;
	      cx2 = (buf2 >> 16) & 0x0f;
	      cx = (cx0 << 13) | (cx1 << 8) | (cx2 << 4) |
		   (((atBuf0 >> atShift0) & 1) << 3) |
		   (((atBuf1 >> atShift1) & 1) << 2) |
		   (((atBuf2 >> atShift2) & 1) << 1) |
		   ((atBuf3 >> atShift3) & 1);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		  if (aty[0] == 0) {
		    atBuf0 |= 0x8000;
		  }
		  if (aty[1] == 0) {
		    atBuf1 |= 0x8000;
		  }
		  if (aty[2] == 0) {
		    atBuf2 |= 0x8000;
		  }
		  if (aty[3] == 0) {
		    atBuf3 |= 0x8000;
		  }
		}
	      }

	      buf0 <<= 1;
	      buf1 <<= 1;
	      buf2 <<= 1;
	      atBuf0 <<= 1;
	      atBuf1 <<= 1;
	      atBuf2 <<= 1;
	      atBuf3 <<= 1;
	    }
	  }

	} else {
	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p0) {
		buf0 |= *p0++;
	      }
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx0 = (buf0 >> 14) & 0x07;
	      cx1 = (buf1 >> 13) & 0x1f;
	      cx2 = (buf2 >> 16) & 0x0f;
	      cx = (cx0 << 13) | (cx1 << 8) | (cx2 << 4) |
		   (bitmap->getPixel(x + atx[0], y + aty[0]) << 3) |
		   (bitmap->getPixel(x + atx[1], y + aty[1]) << 2) |
		   (bitmap->getPixel(x + atx[2], y + aty[2]) << 1) |
		   bitmap->getPixel(x + atx[3], y + aty[3]);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		}
	      }

	      buf0 <<= 1;
	      buf1 <<= 1;
	      buf2 <<= 1;
	    }
	  }
	}
	break;

      case 1:

	p2 = pp = bitmap->getDataPtr() + y * bitmap->getLineSize();
	buf2 = *p2++ << 8;
	if (y >= 1) {
	  p1 = bitmap->getDataPtr() + (y - 1) * bitmap->getLineSize();
	  buf1 = *p1++ << 8;
	  if (y >= 2) {
	    p0 = bitmap->getDataPtr() + (y - 2) * bitmap->getLineSize();
	    buf0 = *p0++ << 8;
	  } else {
	    p0 = NULL;
	    buf0 = 0;
	  }
	} else {
	  p1 = p0 = NULL;
	  buf1 = buf0 = 0;
	}

	if (atx[0] >= -8 && atx[0] <= 8) {
	  if (y + aty[0] >= 0) {
	    atP0 = bitmap->getDataPtr() + (y + aty[0]) * bitmap->getLineSize();
	    atBuf0 = *atP0++ << 8;
	  } else {
	    atP0 = NULL;
	    atBuf0 = 0;
	  }
	  atShift0 = 15 - atx[0];

	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p0) {
		buf0 |= *p0++;
	      }
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	      if (atP0) {
		atBuf0 |= *atP0++;
	      }
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx0 = (buf0 >> 13) & 0x0f;
	      cx1 = (buf1 >> 13) & 0x1f;
	      cx2 = (buf2 >> 16) & 0x07;
	      cx = (cx0 << 9) | (cx1 << 4) | (cx2 << 1) |
		   ((atBuf0 >> atShift0) & 1);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		  if (aty[0] == 0) {
		    atBuf0 |= 0x8000;
		  }
		}
	      }

	      buf0 <<= 1;
	      buf1 <<= 1;
	      buf2 <<= 1;
	      atBuf0 <<= 1;
	    }
	  }

	} else {
	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p0) {
		buf0 |= *p0++;
	      }
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx0 = (buf0 >> 13) & 0x0f;
	      cx1 = (buf1 >> 13) & 0x1f;
	      cx2 = (buf2 >> 16) & 0x07;
	      cx = (cx0 << 9) | (cx1 << 4) | (cx2 << 1) |
		   bitmap->getPixel(x + atx[0], y + aty[0]);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		}
	      }

	      buf0 <<= 1;
	      buf1 <<= 1;
	      buf2 <<= 1;
	    }
	  }
	}
	break;

      case 2:

	p2 = pp = bitmap->getDataPtr() + y * bitmap->getLineSize();
	buf2 = *p2++ << 8;
	if (y >= 1) {
	  p1 = bitmap->getDataPtr() + (y - 1) * bitmap->getLineSize();
	  buf1 = *p1++ << 8;
	  if (y >= 2) {
	    p0 = bitmap->getDataPtr() + (y - 2) * bitmap->getLineSize();
	    buf0 = *p0++ << 8;
	  } else {
	    p0 = NULL;
	    buf0 = 0;
	  }
	} else {
	  p1 = p0 = NULL;
	  buf1 = buf0 = 0;
	}

	if (atx[0] >= -8 && atx[0] <= 8) {
	  if (y + aty[0] >= 0) {
	    atP0 = bitmap->getDataPtr() + (y + aty[0]) * bitmap->getLineSize();
	    atBuf0 = *atP0++ << 8;
	  } else {
	    atP0 = NULL;
	    atBuf0 = 0;
	  }
	  atShift0 = 15 - atx[0];

	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p0) {
		buf0 |= *p0++;
	      }
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	      if (atP0) {
		atBuf0 |= *atP0++;
	      }
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx0 = (buf0 >> 14) & 0x07;
	      cx1 = (buf1 >> 14) & 0x0f;
	      cx2 = (buf2 >> 16) & 0x03;
	      cx = (cx0 << 7) | (cx1 << 3) | (cx2 << 1) |
		   ((atBuf0 >> atShift0) & 1);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		  if (aty[0] == 0) {
		    atBuf0 |= 0x8000;
		  }
		}
	      }

	      buf0 <<= 1;
	      buf1 <<= 1;
	      buf2 <<= 1;
	      atBuf0 <<= 1;
	    }
	  }

	} else {
	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p0) {
		buf0 |= *p0++;
	      }
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx0 = (buf0 >> 14) & 0x07;
	      cx1 = (buf1 >> 14) & 0x0f;
	      cx2 = (buf2 >> 16) & 0x03;
	      cx = (cx0 << 7) | (cx1 << 3) | (cx2 << 1) |
		   bitmap->getPixel(x + atx[0], y + aty[0]);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		}
	      }

	      buf0 <<= 1;
	      buf1 <<= 1;
	      buf2 <<= 1;
	    }
	  }
	}
	break;

      case 3:

	p2 = pp = bitmap->getDataPtr() + y * bitmap->getLineSize();
	buf2 = *p2++ << 8;
	if (y >= 1) {
	  p1 = bitmap->getDataPtr() + (y - 1) * bitmap->getLineSize();
	  buf1 = *p1++ << 8;
	} else {
	  p1 = NULL;
	  buf1 = 0;
	}

	if (atx[0] >= -8 && atx[0] <= 8) {
	  if (y + aty[0] >= 0) {
	    atP0 = bitmap->getDataPtr() + (y + aty[0]) * bitmap->getLineSize();
	    atBuf0 = *atP0++ << 8;
	  } else {
	    atP0 = NULL;
	    atBuf0 = 0;
	  }
	  atShift0 = 15 - atx[0];

	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	      if (atP0) {
		atBuf0 |= *atP0++;
	      }
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx1 = (buf1 >> 14) & 0x1f;
	      cx2 = (buf2 >> 16) & 0x0f;
	      cx = (cx1 << 5) | (cx2 << 1) |
		   ((atBuf0 >> atShift0) & 1);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		  if (aty[0] == 0) {
		    atBuf0 |= 0x8000;
		  }
		}
	      }

	      buf1 <<= 1;
	      buf2 <<= 1;
	      atBuf0 <<= 1;
	    }
	  }

	} else {
	  for (x0 = 0, x = 0; x0 < w; x0 += 8, ++pp) {
	    if (x0 + 8 < w) {
	      if (p1) {
		buf1 |= *p1++;
	      }
	      buf2 |= *p2++;
	    }
	    for (x1 = 0, mask = 0x80; x1 < 8 && x < w; ++x1, ++x, mask >>= 1) {

	      cx1 = (buf1 >> 14) & 0x1f;
	      cx2 = (buf2 >> 16) & 0x0f;
	      cx = (cx1 << 5) | (cx2 << 1) |
		   bitmap->getPixel(x + atx[0], y + aty[0]);

	      if (!(useSkip && skip->getPixel(x, y))) {

		if ((pix = arithDecoder->decodeBit(cx, genericRegionStats))) {
		  *pp |= mask;
		  buf2 |= 0x8000;
		}
	      }

	      buf1 <<= 1;
	      buf2 <<= 1;
	    }
	  }
	}
	break;
      }
    }
  }

  return bitmap;
}
