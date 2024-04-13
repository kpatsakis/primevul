void DCTStream::decodeImage() {
  int dataIn[64];
  Guchar dataOut[64];
  Gushort *quantTable;
  int pY, pCb, pCr, pR, pG, pB;
  int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, cc, i;
  int h, v, horiz, vert, hSub, vSub;
  int *p0, *p1, *p2;

  for (y1 = 0; y1 < bufHeight; y1 += mcuHeight) {
    for (x1 = 0; x1 < bufWidth; x1 += mcuWidth) {
      for (cc = 0; cc < numComps; ++cc) {
	quantTable = quantTables[compInfo[cc].quantTable];
	h = compInfo[cc].hSample;
	v = compInfo[cc].vSample;
	horiz = mcuWidth / h;
	vert = mcuHeight / v;
	hSub = horiz / 8;
	vSub = vert / 8;
	for (y2 = 0; y2 < mcuHeight; y2 += vert) {
	  for (x2 = 0; x2 < mcuWidth; x2 += horiz) {

	    p1 = &frameBuf[cc][(y1+y2) * bufWidth + (x1+x2)];
	    for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8) {
	      dataIn[i]   = p1[0];
	      dataIn[i+1] = p1[1];
	      dataIn[i+2] = p1[2];
	      dataIn[i+3] = p1[3];
	      dataIn[i+4] = p1[4];
	      dataIn[i+5] = p1[5];
	      dataIn[i+6] = p1[6];
	      dataIn[i+7] = p1[7];
	      p1 += bufWidth * vSub;
	    }

	    transformDataUnit(quantTable, dataIn, dataOut);

	    p1 = &frameBuf[cc][(y1+y2) * bufWidth + (x1+x2)];
	    if (hSub == 1 && vSub == 1) {
	      for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8) {
		p1[0] = dataOut[i] & 0xff;
		p1[1] = dataOut[i+1] & 0xff;
		p1[2] = dataOut[i+2] & 0xff;
		p1[3] = dataOut[i+3] & 0xff;
		p1[4] = dataOut[i+4] & 0xff;
		p1[5] = dataOut[i+5] & 0xff;
		p1[6] = dataOut[i+6] & 0xff;
		p1[7] = dataOut[i+7] & 0xff;
		p1 += bufWidth;
	      }
	    } else if (hSub == 2 && vSub == 2) {
	      p2 = p1 + bufWidth;
	      for (y3 = 0, i = 0; y3 < 16; y3 += 2, i += 8) {
		p1[0] = p1[1] = p2[0] = p2[1] = dataOut[i] & 0xff;
		p1[2] = p1[3] = p2[2] = p2[3] = dataOut[i+1] & 0xff;
		p1[4] = p1[5] = p2[4] = p2[5] = dataOut[i+2] & 0xff;
		p1[6] = p1[7] = p2[6] = p2[7] = dataOut[i+3] & 0xff;
		p1[8] = p1[9] = p2[8] = p2[9] = dataOut[i+4] & 0xff;
		p1[10] = p1[11] = p2[10] = p2[11] = dataOut[i+5] & 0xff;
		p1[12] = p1[13] = p2[12] = p2[13] = dataOut[i+6] & 0xff;
		p1[14] = p1[15] = p2[14] = p2[15] = dataOut[i+7] & 0xff;
		p1 += bufWidth * 2;
		p2 += bufWidth * 2;
	      }
	    } else {
	      i = 0;
	      for (y3 = 0, y4 = 0; y3 < 8; ++y3, y4 += vSub) {
		for (x3 = 0, x4 = 0; x3 < 8; ++x3, x4 += hSub) {
		  p2 = p1 + x4;
		  for (y5 = 0; y5 < vSub; ++y5) {
		    for (x5 = 0; x5 < hSub; ++x5) {
		      p2[x5] = dataOut[i] & 0xff;
		    }
		    p2 += bufWidth;
		  }
		  ++i;
		}
		p1 += bufWidth * vSub;
	      }
	    }
	  }
	}
      }

      if (colorXform) {
	if (numComps == 3) {
	  for (y2 = 0; y2 < mcuHeight; ++y2) {
	    p0 = &frameBuf[0][(y1+y2) * bufWidth + x1];
	    p1 = &frameBuf[1][(y1+y2) * bufWidth + x1];
	    p2 = &frameBuf[2][(y1+y2) * bufWidth + x1];
	    for (x2 = 0; x2 < mcuWidth; ++x2) {
	      pY = *p0;
	      pCb = *p1 - 128;
	      pCr = *p2 - 128;
	      pR = ((pY << 16) + dctCrToR * pCr + 32768) >> 16;
	      *p0++ = dctClip[dctClipOffset + pR];
	      pG = ((pY << 16) + dctCbToG * pCb + dctCrToG * pCr +
		    32768) >> 16;
	      *p1++ = dctClip[dctClipOffset + pG];
	      pB = ((pY << 16) + dctCbToB * pCb + 32768) >> 16;
	      *p2++ = dctClip[dctClipOffset + pB];
	    }
	  }
	} else if (numComps == 4) {
	  for (y2 = 0; y2 < mcuHeight; ++y2) {
	    p0 = &frameBuf[0][(y1+y2) * bufWidth + x1];
	    p1 = &frameBuf[1][(y1+y2) * bufWidth + x1];
	    p2 = &frameBuf[2][(y1+y2) * bufWidth + x1];
	    for (x2 = 0; x2 < mcuWidth; ++x2) {
	      pY = *p0;
	      pCb = *p1 - 128;
	      pCr = *p2 - 128;
	      pR = ((pY << 16) + dctCrToR * pCr + 32768) >> 16;
	      *p0++ = 255 - dctClip[dctClipOffset + pR];
	      pG = ((pY << 16) + dctCbToG * pCb + dctCrToG * pCr +
		    32768) >> 16;
	      *p1++ = 255 - dctClip[dctClipOffset + pG];
	      pB = ((pY << 16) + dctCbToB * pCb + 32768) >> 16;
	      *p2++ = 255 - dctClip[dctClipOffset + pB];
	    }
	  }
	}
      }
    }
  }
}
