ProcRenderCreateCursor (ClientPtr client)
{
    REQUEST(xRenderCreateCursorReq);
    PicturePtr	    pSrc;
    ScreenPtr	    pScreen;
    unsigned short  width, height;
    CARD32	    *argbbits, *argb;
    unsigned char   *srcbits, *srcline;
    unsigned char   *mskbits, *mskline;
    int		    stride;
    int		    x, y;
    int		    nbytes_mono;
    CursorMetricRec cm;
    CursorPtr	    pCursor;
    CARD32	    twocolor[3];
    int		    rc, ncolor;

    REQUEST_SIZE_MATCH (xRenderCreateCursorReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);
    
    VERIFY_PICTURE (pSrc, stuff->src, client, DixReadAccess);
    if (!pSrc->pDrawable)
        return BadDrawable;
    pScreen = pSrc->pDrawable->pScreen;
    width = pSrc->pDrawable->width;
    height = pSrc->pDrawable->height;
    if (height && width > UINT32_MAX/(height*sizeof(CARD32)))
	return BadAlloc;
    if ( stuff->x > width 
      || stuff->y > height )
	return BadMatch;
    argbbits = malloc(width * height * sizeof (CARD32));
    if (!argbbits)
	return BadAlloc;
    
    stride = BitmapBytePad(width);
    nbytes_mono = stride*height;
    srcbits = calloc(1, nbytes_mono);
    if (!srcbits)
    {
	free(argbbits);
	return BadAlloc;
    }
    mskbits = calloc(1, nbytes_mono);
    if (!mskbits)
    {
	free(argbbits);
	free(srcbits);
	return BadAlloc;
    }

    if (pSrc->format == PICT_a8r8g8b8)
    {
	(*pScreen->GetImage) (pSrc->pDrawable,
			      0, 0, width, height, ZPixmap,
			      0xffffffff, (pointer) argbbits);
    }
    else
    {
	PixmapPtr	pPixmap;
	PicturePtr	pPicture;
	PictFormatPtr	pFormat;
	int		error;

	pFormat = PictureMatchFormat (pScreen, 32, PICT_a8r8g8b8);
	if (!pFormat)
	{
	    free(argbbits);
	    free(srcbits);
	    free(mskbits);
	    return BadImplementation;
	}
	pPixmap = (*pScreen->CreatePixmap) (pScreen, width, height, 32,
					    CREATE_PIXMAP_USAGE_SCRATCH);
	if (!pPixmap)
	{
	    free(argbbits);
	    free(srcbits);
	    free(mskbits);
	    return BadAlloc;
	}
	pPicture = CreatePicture (0, &pPixmap->drawable, pFormat, 0, 0, 
				  client, &error);
	if (!pPicture)
	{
	    free(argbbits);
	    free(srcbits);
	    free(mskbits);
	    return error;
	}
	(*pScreen->DestroyPixmap) (pPixmap);
	CompositePicture (PictOpSrc,
			  pSrc, 0, pPicture,
			  0, 0, 0, 0, 0, 0, width, height);
	(*pScreen->GetImage) (pPicture->pDrawable,
			      0, 0, width, height, ZPixmap,
			      0xffffffff, (pointer) argbbits);
	FreePicture (pPicture, 0);
    }
    /*
     * Check whether the cursor can be directly supported by 
     * the core cursor code
     */
    ncolor = 0;
    argb = argbbits;
    for (y = 0; ncolor <= 2 && y < height; y++)
    {
	for (x = 0; ncolor <= 2 && x < width; x++)
	{
	    CARD32  p = *argb++;
	    CARD32  a = (p >> 24);

	    if (a == 0)	    /* transparent */
		continue;
	    if (a == 0xff)  /* opaque */
	    {
		int n;
		for (n = 0; n < ncolor; n++)
		    if (p == twocolor[n])
			break;
		if (n == ncolor)
		    twocolor[ncolor++] = p;
	    }
	    else
		ncolor = 3;
	}
    }
    
    /*
     * Convert argb image to two plane cursor
     */
    srcline = srcbits;
    mskline = mskbits;
    argb = argbbits;
    for (y = 0; y < height; y++)
    {
	for (x = 0; x < width; x++)
	{
	    CARD32  p = *argb++;

	    if (ncolor <= 2)
	    {
		CARD32	a = ((p >> 24));

		RenderSetBit (mskline, x, a != 0);
		RenderSetBit (srcline, x, a != 0 && p == twocolor[0]);
	    }
	    else
	    {
		CARD32	a = ((p >> 24) * DITHER_SIZE + 127) / 255;
		CARD32	i = ((CvtR8G8B8toY15(p) >> 7) * DITHER_SIZE + 127) / 255;
		CARD32	d = orderedDither[y&(DITHER_DIM-1)][x&(DITHER_DIM-1)];
		/* Set mask from dithered alpha value */
		RenderSetBit(mskline, x, a > d);
		/* Set src from dithered intensity value */
		RenderSetBit(srcline, x, a > d && i <= d);
	    }
	}
	srcline += stride;
	mskline += stride;
    }
    /*
     * Dither to white and black if the cursor has more than two colors
     */
    if (ncolor > 2)
    {
	twocolor[0] = 0xff000000;
	twocolor[1] = 0xffffffff;
    }
    else
    {
	free(argbbits);
	argbbits = 0;
    }
    
#define GetByte(p,s)	(((p) >> (s)) & 0xff)
#define GetColor(p,s)	(GetByte(p,s) | (GetByte(p,s) << 8))
    
    cm.width = width;
    cm.height = height;
    cm.xhot = stuff->x;
    cm.yhot = stuff->y;
    rc = AllocARGBCursor(srcbits, mskbits, argbbits, &cm,
			 GetColor(twocolor[0], 16),
			 GetColor(twocolor[0], 8),
			 GetColor(twocolor[0], 0),
			 GetColor(twocolor[1], 16),
			 GetColor(twocolor[1], 8),
			 GetColor(twocolor[1], 0),
			 &pCursor, client, stuff->cid);
    if (rc != Success)
	return rc;
    if (!AddResource(stuff->cid, RT_CURSOR, (pointer)pCursor))
	return BadAlloc;

    return Success;
}
