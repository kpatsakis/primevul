fbCompositeRect (const FbComposeData *data, CARD32 *scanline_buffer)
{
    CARD32 *src_buffer = scanline_buffer;
    CARD32 *dest_buffer = src_buffer + data->width;
    int i;
    scanStoreProc store;
    scanFetchProc fetchSrc = NULL, fetchMask = NULL, fetchDest = NULL;
    unsigned int srcClass = SourcePictClassUnknown;
    unsigned int maskClass = SourcePictClassUnknown;
    FbBits *bits;
    FbStride stride;
    int xoff, yoff;
    
    if (data->op == PictOpClear)
        fetchSrc = NULL;
    else if (!data->src->pDrawable) {
        if (data->src->pSourcePict)
	{
            fetchSrc = fbFetchSourcePict;
	    srcClass = SourcePictureClassify (data->src,
					      data->xSrc, data->ySrc,
					      data->width, data->height);
	}
    } else if (data->src->alphaMap)
        fetchSrc = fbFetchExternalAlpha;
    else if (data->src->repeatType == RepeatNormal &&
             data->src->pDrawable->width == 1 && data->src->pDrawable->height == 1)
    {
        fetchSrc = fbFetchSolid;
	srcClass = SourcePictClassHorizontal;
    }
    else if (!data->src->transform && data->src->filter != PictFilterConvolution)
        fetchSrc = fbFetch;
    else
        fetchSrc = fbFetchTransformed;
    
    if (data->mask && data->op != PictOpClear) {
        if (!data->mask->pDrawable) {
            if (data->mask->pSourcePict)
                fetchMask = fbFetchSourcePict;
        } else if (data->mask->alphaMap)
	{
            fetchMask = fbFetchExternalAlpha;
	    maskClass = SourcePictureClassify (data->mask,
					       data->xMask, data->yMask,
					       data->width, data->height);
	}
        else if (data->mask->repeatType == RepeatNormal
                 && data->mask->pDrawable->width == 1 && data->mask->pDrawable->height == 1)
	{
	    fetchMask = fbFetchSolid;
	    maskClass = SourcePictClassHorizontal;
	}
        else if (!data->mask->transform && data->mask->filter != PictFilterConvolution)
            fetchMask = fbFetch;
        else
            fetchMask = fbFetchTransformed;
    } else {
        fetchMask = NULL;
    }
    
    if (data->dest->alphaMap)
    {
	fetchDest = fbFetchExternalAlpha;
	store = fbStoreExternalAlpha;
	
	if (data->op == PictOpClear || data->op == PictOpSrc)
	    fetchDest = NULL;
    }
    else
    {
	fetchDest = fbFetch;
	store = fbStore;
	
	switch (data->op) {
	case PictOpClear:
	case PictOpSrc:
	    fetchDest = NULL;
	    /* fall-through */
	case PictOpAdd:
	case PictOpOver:
	    switch (data->dest->format) {
	    case PICT_a8r8g8b8:
	    case PICT_x8r8g8b8:
		store = NULL;
		break;
	    default:
		break;
	    }
	    break;
	}
    }
    
    if (!store)
    {
	int bpp;
	
	fbGetDrawable (data->dest->pDrawable, bits, stride, bpp, xoff, yoff);
    }
    else
    {
	bits = NULL;
	stride = 0;
	xoff = yoff = 0;
    }
    
    if (fetchSrc		   &&
	fetchMask		   &&
	data->mask		   &&
	data->mask->componentAlpha &&
	PICT_FORMAT_RGB (data->mask->format))
    {
	CARD32 *mask_buffer = dest_buffer + data->width;
	CombineFuncC compose = composeFunctions.combineC[data->op];
	if (!compose)
	    return;
	
	for (i = 0; i < data->height; ++i) {
	    /* fill first half of scanline with source */
	    if (fetchSrc)
	    {
		if (fetchMask)
		{
		    /* fetch mask before source so that fetching of
		       source can be optimized */
		    fetchMask (data->mask, data->xMask, data->yMask + i,
			       data->width, mask_buffer, 0, 0);
		    
		    if (maskClass == SourcePictClassHorizontal)
			fetchMask = NULL;
		}
		
		if (srcClass == SourcePictClassHorizontal)
		{
		    fetchSrc (data->src, data->xSrc, data->ySrc + i,
			      data->width, src_buffer, 0, 0);
		    fetchSrc = NULL;
		}
		else
		{
		    fetchSrc (data->src, data->xSrc, data->ySrc + i,
			      data->width, src_buffer, mask_buffer,
			      0xffffffff);
		}
	    }
	    else if (fetchMask)
	    {
		fetchMask (data->mask, data->xMask, data->yMask + i,
			   data->width, mask_buffer, 0, 0);
	    }
	    
	    if (store)
	    {
		/* fill dest into second half of scanline */
		if (fetchDest)
		    fetchDest (data->dest, data->xDest, data->yDest + i,
			       data->width, dest_buffer, 0, 0);
		
		/* blend */
		compose (dest_buffer, src_buffer, mask_buffer, data->width);
		
		/* write back */
		store (data->dest, data->xDest, data->yDest + i, data->width,
		       dest_buffer);
	    }
	    else
	    {
		/* blend */
		compose (bits + (data->yDest + i+ yoff) * stride +
			 data->xDest + xoff,
			 src_buffer, mask_buffer, data->width);
	    }
	}
    }
    else
    {
	CARD32 *src_mask_buffer = 0, *mask_buffer = 0;
	CombineFuncU compose = composeFunctions.combineU[data->op];
	if (!compose)
	    return;
	
	if (fetchMask)
	    mask_buffer = dest_buffer + data->width;
	
	for (i = 0; i < data->height; ++i) {
	    /* fill first half of scanline with source */
	    if (fetchSrc)
	    {
		if (fetchMask)
		{
		    /* fetch mask before source so that fetching of
		       source can be optimized */
		    fetchMask (data->mask, data->xMask, data->yMask + i,
			       data->width, mask_buffer, 0, 0);
		    
		    if (maskClass == SourcePictClassHorizontal)
			fetchMask = NULL;
		}
		
		if (srcClass == SourcePictClassHorizontal)
		{
		    fetchSrc (data->src, data->xSrc, data->ySrc + i,
			      data->width, src_buffer, 0, 0);
		    
		    if (mask_buffer)
		    {
			fbCombineInU (mask_buffer, src_buffer, data->width);
			src_mask_buffer = mask_buffer;
		    }
		    else
			src_mask_buffer = src_buffer;
		    
		    fetchSrc = NULL;
		}
		else
		{
		    fetchSrc (data->src, data->xSrc, data->ySrc + i,
			      data->width, src_buffer, mask_buffer,
			      0xff000000);
		    
		    if (mask_buffer)
			composeFunctions.combineMaskU (src_buffer,
						       mask_buffer,
						       data->width);
		    
		    src_mask_buffer = src_buffer;
		}
	    }
	    else if (fetchMask)
	    {
		fetchMask (data->mask, data->xMask, data->yMask + i,
			   data->width, mask_buffer, 0, 0);
		
		fbCombineInU (mask_buffer, src_buffer, data->width);
		
		src_mask_buffer = mask_buffer;
	    }
	    
	    if (store)
	    {
		/* fill dest into second half of scanline */
		if (fetchDest)
		    fetchDest (data->dest, data->xDest, data->yDest + i,
			       data->width, dest_buffer, 0, 0);
		
		/* blend */
		compose (dest_buffer, src_mask_buffer, data->width);
		
		/* write back */
		store (data->dest, data->xDest, data->yDest + i, data->width,
		       dest_buffer);
	    }
	    else
	    {
		/* blend */
		compose (bits + (data->yDest + i+ yoff) * stride +
			 data->xDest + xoff,
			 src_mask_buffer, data->width);
	    }
	}
    }

    if (!store)
	fbFinishAccess (data->dest->pDrawable);
}
