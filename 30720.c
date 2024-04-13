void DCTStream::reset() {
  int i, j;

  dctReset(gFalse);

  if (!readHeader()) {
    y = height;
    return;
  }

  if (numComps == 1) {
    compInfo[0].hSample = compInfo[0].vSample = 1;
  }
  mcuWidth = compInfo[0].hSample;
  mcuHeight = compInfo[0].vSample;
  for (i = 1; i < numComps; ++i) {
    if (compInfo[i].hSample > mcuWidth) {
      mcuWidth = compInfo[i].hSample;
    }
    if (compInfo[i].vSample > mcuHeight) {
      mcuHeight = compInfo[i].vSample;
    }
  }
  mcuWidth *= 8;
  mcuHeight *= 8;

  if (colorXform == -1) {
    if (numComps == 3) {
      if (gotJFIFMarker) {
	colorXform = 1;
      } else if (compInfo[0].id == 82 && compInfo[1].id == 71 &&
		 compInfo[2].id == 66) { // ASCII "RGB"
	colorXform = 0;
      } else {
	colorXform = 1;
      }
    } else {
      colorXform = 0;
    }
  }

  if (progressive || !interleaved) {

    bufWidth = ((width + mcuWidth - 1) / mcuWidth) * mcuWidth;
    bufHeight = ((height + mcuHeight - 1) / mcuHeight) * mcuHeight;
    if (bufWidth <= 0 || bufHeight <= 0 ||
	bufWidth > INT_MAX / bufWidth / (int)sizeof(int)) {
      error(errSyntaxError, getPos(), "Invalid image size in DCT stream");
      y = height;
      return;
    }
    for (i = 0; i < numComps; ++i) {
      frameBuf[i] = (int *)gmallocn(bufWidth * bufHeight, sizeof(int));
      memset(frameBuf[i], 0, bufWidth * bufHeight * sizeof(int));
    }

    do {
      restartMarker = 0xd0;
      restart();
      readScan();
    } while (readHeader());

    decodeImage();

    comp = 0;
    x = 0;
    y = 0;

  } else {

    bufWidth = ((width + mcuWidth - 1) / mcuWidth) * mcuWidth;
    for (i = 0; i < numComps; ++i) {
      for (j = 0; j < mcuHeight; ++j) {
	rowBuf[i][j] = (Guchar *)gmallocn(bufWidth, sizeof(Guchar));
      }
    }

    comp = 0;
    x = 0;
    y = 0;
    dy = mcuHeight;

    restartMarker = 0xd0;
    restart();
  }
}
