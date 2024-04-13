void DCTStream::readScan() {
  int data[64];
  int x1, y1, dx1, dy1, x2, y2, y3, cc, i;
  int h, v, horiz, vert, vSub;
  int *p1;
  int c;

  if (scanInfo.numComps == 1) {
    for (cc = 0; cc < numComps; ++cc) {
      if (scanInfo.comp[cc]) {
	break;
      }
    }
    dx1 = mcuWidth / compInfo[cc].hSample;
    dy1 = mcuHeight / compInfo[cc].vSample;
  } else {
    dx1 = mcuWidth;
    dy1 = mcuHeight;
  }

  for (y1 = 0; y1 < height; y1 += dy1) {
    for (x1 = 0; x1 < width; x1 += dx1) {

      if (restartInterval > 0 && restartCtr == 0) {
	c = readMarker();
	if (c != restartMarker) {
	  error(errSyntaxError, getPos(),
		"Bad DCT data: incorrect restart marker");
	  return;
	}
	if (++restartMarker == 0xd8) {
	  restartMarker = 0xd0;
	}
	restart();
      }

      for (cc = 0; cc < numComps; ++cc) {
	if (!scanInfo.comp[cc]) {
	  continue;
	}

	h = compInfo[cc].hSample;
	v = compInfo[cc].vSample;
	horiz = mcuWidth / h;
	vert = mcuHeight / v;
	vSub = vert / 8;
	for (y2 = 0; y2 < dy1; y2 += vert) {
	  for (x2 = 0; x2 < dx1; x2 += horiz) {

	    p1 = &frameBuf[cc][(y1+y2) * bufWidth + (x1+x2)];
	    for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8) {
	      data[i] = p1[0];
	      data[i+1] = p1[1];
	      data[i+2] = p1[2];
	      data[i+3] = p1[3];
	      data[i+4] = p1[4];
	      data[i+5] = p1[5];
	      data[i+6] = p1[6];
	      data[i+7] = p1[7];
	      p1 += bufWidth * vSub;
	    }

	    if (progressive) {
	      if (!readProgressiveDataUnit(
		       &dcHuffTables[scanInfo.dcHuffTable[cc]],
		       &acHuffTables[scanInfo.acHuffTable[cc]],
		       &compInfo[cc].prevDC,
		       data)) {
		return;
	      }
	    } else {
	      if (!readDataUnit(&dcHuffTables[scanInfo.dcHuffTable[cc]],
				&acHuffTables[scanInfo.acHuffTable[cc]],
				&compInfo[cc].prevDC,
				data)) {
		return;
	      }
	    }

	    p1 = &frameBuf[cc][(y1+y2) * bufWidth + (x1+x2)];
	    for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8) {
	      p1[0] = data[i];
	      p1[1] = data[i+1];
	      p1[2] = data[i+2];
	      p1[3] = data[i+3];
	      p1[4] = data[i+4];
	      p1[5] = data[i+5];
	      p1[6] = data[i+6];
	      p1[7] = data[i+7];
	      p1 += bufWidth * vSub;
	    }
	  }
	}
      }
      --restartCtr;
    }
  }
}
