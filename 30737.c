void DCTStream::transformDataUnit(Gushort *quantTable,
				  int dataIn[64], Guchar dataOut[64]) {
  int v0, v1, v2, v3, v4, v5, v6, v7, t;
  int *p;
  int i;

  for (i = 0; i < 64; ++i) {
    dataIn[i] *= quantTable[i];
  }

  for (i = 0; i < 64; i += 8) {
    p = dataIn + i;

    if (p[1] == 0 && p[2] == 0 && p[3] == 0 &&
	p[4] == 0 && p[5] == 0 && p[6] == 0 && p[7] == 0) {
      t = (dctSqrt2 * p[0] + 512) >> 10;
      p[0] = t;
      p[1] = t;
      p[2] = t;
      p[3] = t;
      p[4] = t;
      p[5] = t;
      p[6] = t;
      p[7] = t;
      continue;
    }

    v0 = (dctSqrt2 * p[0] + 128) >> 8;
    v1 = (dctSqrt2 * p[4] + 128) >> 8;
    v2 = p[2];
    v3 = p[6];
    v4 = (dctSqrt1d2 * (p[1] - p[7]) + 128) >> 8;
    v7 = (dctSqrt1d2 * (p[1] + p[7]) + 128) >> 8;
    v5 = p[3] << 4;
    v6 = p[5] << 4;

    t = (v0 - v1+ 1) >> 1;
    v0 = (v0 + v1 + 1) >> 1;
    v1 = t;
    t = (v2 * dctSin6 + v3 * dctCos6 + 128) >> 8;
    v2 = (v2 * dctCos6 - v3 * dctSin6 + 128) >> 8;
    v3 = t;
    t = (v4 - v6 + 1) >> 1;
    v4 = (v4 + v6 + 1) >> 1;
    v6 = t;
    t = (v7 + v5 + 1) >> 1;
    v5 = (v7 - v5 + 1) >> 1;
    v7 = t;

    t = (v0 - v3 + 1) >> 1;
    v0 = (v0 + v3 + 1) >> 1;
    v3 = t;
    t = (v1 - v2 + 1) >> 1;
    v1 = (v1 + v2 + 1) >> 1;
    v2 = t;
    t = (v4 * dctSin3 + v7 * dctCos3 + 2048) >> 12;
    v4 = (v4 * dctCos3 - v7 * dctSin3 + 2048) >> 12;
    v7 = t;
    t = (v5 * dctSin1 + v6 * dctCos1 + 2048) >> 12;
    v5 = (v5 * dctCos1 - v6 * dctSin1 + 2048) >> 12;
    v6 = t;

    p[0] = v0 + v7;
    p[7] = v0 - v7;
    p[1] = v1 + v6;
    p[6] = v1 - v6;
    p[2] = v2 + v5;
    p[5] = v2 - v5;
    p[3] = v3 + v4;
    p[4] = v3 - v4;
  }

  for (i = 0; i < 8; ++i) {
    p = dataIn + i;

    if (p[1*8] == 0 && p[2*8] == 0 && p[3*8] == 0 &&
	p[4*8] == 0 && p[5*8] == 0 && p[6*8] == 0 && p[7*8] == 0) {
      t = (dctSqrt2 * dataIn[i+0] + 8192) >> 14;
      p[0*8] = t;
      p[1*8] = t;
      p[2*8] = t;
      p[3*8] = t;
      p[4*8] = t;
      p[5*8] = t;
      p[6*8] = t;
      p[7*8] = t;
      continue;
    }

    v0 = (dctSqrt2 * p[0*8] + 2048) >> 12;
    v1 = (dctSqrt2 * p[4*8] + 2048) >> 12;
    v2 = p[2*8];
    v3 = p[6*8];
    v4 = (dctSqrt1d2 * (p[1*8] - p[7*8]) + 2048) >> 12;
    v7 = (dctSqrt1d2 * (p[1*8] + p[7*8]) + 2048) >> 12;
    v5 = p[3*8];
    v6 = p[5*8];

    t = (v0 - v1 + 1) >> 1;
    v0 = (v0 + v1 + 1) >> 1;
    v1 = t;
    t = (v2 * dctSin6 + v3 * dctCos6 + 2048) >> 12;
    v2 = (v2 * dctCos6 - v3 * dctSin6 + 2048) >> 12;
    v3 = t;
    t = (v4 - v6 + 1) >> 1;
    v4 = (v4 + v6 + 1) >> 1;
    v6 = t;
    t = (v7 + v5 + 1) >> 1;
    v5 = (v7 - v5 + 1) >> 1;
    v7 = t;

    t = (v0 - v3 + 1) >> 1;
    v0 = (v0 + v3 + 1) >> 1;
    v3 = t;
    t = (v1 - v2 + 1) >> 1;
    v1 = (v1 + v2 + 1) >> 1;
    v2 = t;
    t = (v4 * dctSin3 + v7 * dctCos3 + 2048) >> 12;
    v4 = (v4 * dctCos3 - v7 * dctSin3 + 2048) >> 12;
    v7 = t;
    t = (v5 * dctSin1 + v6 * dctCos1 + 2048) >> 12;
    v5 = (v5 * dctCos1 - v6 * dctSin1 + 2048) >> 12;
    v6 = t;

    p[0*8] = v0 + v7;
    p[7*8] = v0 - v7;
    p[1*8] = v1 + v6;
    p[6*8] = v1 - v6;
    p[2*8] = v2 + v5;
    p[5*8] = v2 - v5;
    p[3*8] = v3 + v4;
    p[4*8] = v3 - v4;
  }

  for (i = 0; i < 64; ++i) {
    dataOut[i] = dctClip[dctClipOffset + 128 + ((dataIn[i] + 8) >> 4)];
  }
}
