GBool DCTStream::readMCURow() {
  int data1[64];
  Guchar data2[64];
  Guchar *p1, *p2;
  int pY, pCb, pCr, pR, pG, pB;
  int h, v, horiz, vert, hSub, vSub;
  int x1, x2, y2, x3, y3, x4, y4, x5, y5, cc, i;
  int c;

  for (x1 = 0; x1 < width; x1 += mcuWidth) {

    if (restartInterval > 0 && restartCtr == 0) {
      c = readMarker();
      if (c != restartMarker) {
	error(errSyntaxError, getPos(),
	      "Bad DCT data: incorrect restart marker");
	return gFalse;
      }
      if (++restartMarker == 0xd8)
	restartMarker = 0xd0;
      restart();
    }

    for (cc = 0; cc < numComps; ++cc) {
      h = compInfo[cc].hSample;
      v = compInfo[cc].vSample;
      horiz = mcuWidth / h;
      vert = mcuHeight / v;
      hSub = horiz / 8;
      vSub = vert / 8;
      for (y2 = 0; y2 < mcuHeight; y2 += vert) {
	for (x2 = 0; x2 < mcuWidth; x2 += horiz) {
	  if (unlikely(scanInfo.dcHuffTable[cc] >= 4) || unlikely(scanInfo.acHuffTable[cc] >= 4)) {
	    return gFalse;
	  }
	  if (!readDataUnit(&dcHuffTables[scanInfo.dcHuffTable[cc]],
			    &acHuffTables[scanInfo.acHuffTable[cc]],
			    &compInfo[cc].prevDC,
			    data1)) {
	    return gFalse;
	  }
	  transformDataUnit(quantTables[compInfo[cc].quantTable],
			    data1, data2);
	  if (hSub == 1 && vSub == 1) {
	    for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8) {
	      p1 = &rowBuf[cc][y2+y3][x1+x2];
	      p1[0] = data2[i];
	      p1[1] = data2[i+1];
	      p1[2] = data2[i+2];
	      p1[3] = data2[i+3];
	      p1[4] = data2[i+4];
	      p1[5] = data2[i+5];
	      p1[6] = data2[i+6];
	      p1[7] = data2[i+7];
	    }
	  } else if (hSub == 2 && vSub == 2) {
	    for (y3 = 0, i = 0; y3 < 16; y3 += 2, i += 8) {
	      p1 = &rowBuf[cc][y2+y3][x1+x2];
	      p2 = &rowBuf[cc][y2+y3+1][x1+x2];
	      p1[0] = p1[1] = p2[0] = p2[1] = data2[i];
	      p1[2] = p1[3] = p2[2] = p2[3] = data2[i+1];
	      p1[4] = p1[5] = p2[4] = p2[5] = data2[i+2];
	      p1[6] = p1[7] = p2[6] = p2[7] = data2[i+3];
	      p1[8] = p1[9] = p2[8] = p2[9] = data2[i+4];
	      p1[10] = p1[11] = p2[10] = p2[11] = data2[i+5];
	      p1[12] = p1[13] = p2[12] = p2[13] = data2[i+6];
	      p1[14] = p1[15] = p2[14] = p2[15] = data2[i+7];
	    }
	  } else {
	    i = 0;
	    for (y3 = 0, y4 = 0; y3 < 8; ++y3, y4 += vSub) {
	      for (x3 = 0, x4 = 0; x3 < 8; ++x3, x4 += hSub) {
		for (y5 = 0; y5 < vSub; ++y5)
		  for (x5 = 0; x5 < hSub; ++x5)
		    rowBuf[cc][y2+y4+y5][x1+x2+x4+x5] = data2[i];
		++i;
	      }
	    }
	  }
	}
      }
    }
    --restartCtr;

    if (colorXform) {
      if (numComps == 3) {
	for (y2 = 0; y2 < mcuHeight; ++y2) {
	  for (x2 = 0; x2 < mcuWidth; ++x2) {
	    pY = rowBuf[0][y2][x1+x2];
	    pCb = rowBuf[1][y2][x1+x2] - 128;
	    pCr = rowBuf[2][y2][x1+x2] - 128;
	    pR = ((pY << 16) + dctCrToR * pCr + 32768) >> 16;
	    rowBuf[0][y2][x1+x2] = dctClip[dctClipOffset + pR];
	    pG = ((pY << 16) + dctCbToG * pCb + dctCrToG * pCr + 32768) >> 16;
	    rowBuf[1][y2][x1+x2] = dctClip[dctClipOffset + pG];
	    pB = ((pY << 16) + dctCbToB * pCb + 32768) >> 16;
	    rowBuf[2][y2][x1+x2] = dctClip[dctClipOffset + pB];
	  }
	}
      } else if (numComps == 4) {
	for (y2 = 0; y2 < mcuHeight; ++y2) {
	  for (x2 = 0; x2 < mcuWidth; ++x2) {
	    pY = rowBuf[0][y2][x1+x2];
	    pCb = rowBuf[1][y2][x1+x2] - 128;
	    pCr = rowBuf[2][y2][x1+x2] - 128;
	    pR = ((pY << 16) + dctCrToR * pCr + 32768) >> 16;
	    rowBuf[0][y2][x1+x2] = 255 - dctClip[dctClipOffset + pR];
	    pG = ((pY << 16) + dctCbToG * pCb + dctCrToG * pCr + 32768) >> 16;
	    rowBuf[1][y2][x1+x2] = 255 - dctClip[dctClipOffset + pG];
	    pB = ((pY << 16) + dctCbToB * pCb + 32768) >> 16;
	    rowBuf[2][y2][x1+x2] = 255 - dctClip[dctClipOffset + pB];
	  }
	}
      }
    }
  }
  return gTrue;
}
