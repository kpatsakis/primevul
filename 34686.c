bdfReadBitmap(CharInfoPtr pCI, FontFilePtr file, int bit, int byte,
	      int glyph, int scan, CARD32 *sizes)
{
    int         widthBits,
                widthBytes,
                widthHexChars;
    int         height,
                row;
    int         i,
                inLineLen,
                nextByte;
    unsigned char *pInBits,
               *picture,
               *line = NULL;
    unsigned char        lineBuf[BDFLINELEN];

    widthBits = GLYPHWIDTHPIXELS(pCI);
    height = GLYPHHEIGHTPIXELS(pCI);

    widthBytes = BYTES_PER_ROW(widthBits, glyph);
    if (widthBytes * height > 0) {
	picture = malloc(widthBytes * height);
	if (!picture) {
          bdfError("Couldn't allocate picture (%d*%d)\n", widthBytes, height);
	    goto BAILOUT;
      }
    } else
	picture = NULL;
    pCI->bits = (char *) picture;

    if (sizes) {
	for (i = 0; i < GLYPHPADOPTIONS; i++)
	    sizes[i] += BYTES_PER_ROW(widthBits, (1 << i)) * height;
    }
    nextByte = 0;
    widthHexChars = BYTES_PER_ROW(widthBits, 1);

/* 5/31/89 (ef) -- hack, hack, hack.  what *am* I supposed to do with */
/*		0 width characters? */

    for (row = 0; row < height; row++) {
	line = bdfGetLine(file, lineBuf, BDFLINELEN);
	if (!line)
	    break;

	if (widthBits == 0) {
	    if ((!line) || (bdfIsPrefix(line, "ENDCHAR")))
		break;
	    else
		continue;
	}
	pInBits = line;
	inLineLen = strlen((char *) pInBits);

	if (inLineLen & 1) {
	    bdfError("odd number of characters in hex encoding\n");
	    line[inLineLen++] = '0';
	    line[inLineLen] = '\0';
	}
	inLineLen >>= 1;
	i = inLineLen;
	if (i > widthHexChars)
	    i = widthHexChars;
	for (; i > 0; i--, pInBits += 2)
	    picture[nextByte++] = bdfHexByte(pInBits);

	/* pad if line is too short */
	if (inLineLen < widthHexChars) {
	    for (i = widthHexChars - inLineLen; i > 0; i--)
		picture[nextByte++] = 0;
	} else {
	    unsigned char mask;

	    mask = 0xff << (8 - (widthBits & 0x7));
	    if (mask && picture[nextByte - 1] & ~mask) {
		picture[nextByte - 1] &= mask;
	    }
	}

	if (widthBytes > widthHexChars) {
	    i = widthBytes - widthHexChars;
	    while (i-- > 0)
		picture[nextByte++] = 0;
	}
    }

    if ((line && (!bdfIsPrefix(line, "ENDCHAR"))) || (height == 0))
	line = bdfGetLine(file, lineBuf, BDFLINELEN);

    if ((!line) || (!bdfIsPrefix(line, "ENDCHAR"))) {
	bdfError("missing 'ENDCHAR'\n");
	goto BAILOUT;
    }
    if (nextByte != height * widthBytes) {
	bdfError("bytes != rows * bytes_per_row (%d != %d * %d)\n",
		 nextByte, height, widthBytes);
	goto BAILOUT;
    }
    if (picture != NULL) {
	if (bit == LSBFirst)
	    BitOrderInvert(picture, nextByte);
	if (bit != byte) {
	    if (scan == 2)
		TwoByteSwap(picture, nextByte);
	    else if (scan == 4)
		FourByteSwap(picture, nextByte);
	}
    }
    return (TRUE);
BAILOUT:
    if (picture)
	free(picture);
    pCI->bits = NULL;
    return (FALSE);
}
