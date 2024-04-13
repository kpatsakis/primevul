ProcRenderCompositeGlyphs (ClientPtr client)
{
    GlyphSetPtr     glyphSet;
    GlyphSet	    gs;
    PicturePtr      pSrc, pDst;
    PictFormatPtr   pFormat;
    GlyphListRec    listsLocal[NLOCALDELTA];
    GlyphListPtr    lists, listsBase;
    GlyphPtr	    glyphsLocal[NLOCALGLYPH];
    Glyph	    glyph;
    GlyphPtr	    *glyphs, *glyphsBase;
    xGlyphElt	    *elt;
    CARD8	    *buffer, *end;
    int		    nglyph;
    int		    nlist;
    int		    space;
    int		    size;
    int		    rc, n;
    
    REQUEST(xRenderCompositeGlyphsReq);

    REQUEST_AT_LEAST_SIZE(xRenderCompositeGlyphsReq);

    switch (stuff->renderReqType) {
    default:			    size = 1; break;
    case X_RenderCompositeGlyphs16: size = 2; break;
    case X_RenderCompositeGlyphs32: size = 4; break;
    }
	    
    if (!PictOpValid (stuff->op))
    {
	client->errorValue = stuff->op;
	return BadValue;
    }
    VERIFY_PICTURE (pSrc, stuff->src, client, DixReadAccess);
    VERIFY_PICTURE (pDst, stuff->dst, client, DixWriteAccess);
    if (!pDst->pDrawable)
        return BadDrawable;
    if (pSrc->pDrawable && pSrc->pDrawable->pScreen != pDst->pDrawable->pScreen)
	return BadMatch;
    if (stuff->maskFormat)
    {
	rc = dixLookupResourceByType((pointer *)&pFormat, stuff->maskFormat,
				     PictFormatType, client, DixReadAccess);
	if (rc != Success)
	    return rc;
    }
    else
	pFormat = 0;

    rc = dixLookupResourceByType((pointer *)&glyphSet, stuff->glyphset,
				 GlyphSetType, client, DixUseAccess);
    if (rc != Success)
	return rc;

    buffer = (CARD8 *) (stuff + 1);
    end = (CARD8 *) stuff + (client->req_len << 2);
    nglyph = 0;
    nlist = 0;
    while (buffer + sizeof (xGlyphElt) < end)
    {
	elt = (xGlyphElt *) buffer;
	buffer += sizeof (xGlyphElt);
	
	if (elt->len == 0xff)
	{
	    buffer += 4;
	}
	else
	{
	    nlist++;
	    nglyph += elt->len;
	    space = size * elt->len;
	    if (space & 3)
		space += 4 - (space & 3);
	    buffer += space;
	}
    }
    if (nglyph <= NLOCALGLYPH)
	glyphsBase = glyphsLocal;
    else
    {
	glyphsBase = (GlyphPtr *) malloc(nglyph * sizeof (GlyphPtr));
	if (!glyphsBase)
	    return BadAlloc;
    }
    if (nlist <= NLOCALDELTA)
	listsBase = listsLocal;
    else
    {
	listsBase = (GlyphListPtr) malloc(nlist * sizeof (GlyphListRec));
	if (!listsBase)
	    return BadAlloc;
    }
    buffer = (CARD8 *) (stuff + 1);
    glyphs = glyphsBase;
    lists = listsBase;
    while (buffer + sizeof (xGlyphElt) < end)
    {
	elt = (xGlyphElt *) buffer;
	buffer += sizeof (xGlyphElt);
	
	if (elt->len == 0xff)
	{
	    if (buffer + sizeof (GlyphSet) < end)
	    {
                memcpy(&gs, buffer, sizeof(GlyphSet));
		rc = dixLookupResourceByType((pointer *)&glyphSet, gs,
					     GlyphSetType, client,
					     DixUseAccess);
		if (rc != Success)
		{
		    if (glyphsBase != glyphsLocal)
			free(glyphsBase);
		    if (listsBase != listsLocal)
			free(listsBase);
		    return rc;
		}
	    }
	    buffer += 4;
	}
	else
	{
	    lists->xOff = elt->deltax;
	    lists->yOff = elt->deltay;
	    lists->format = glyphSet->format;
	    lists->len = 0;
	    n = elt->len;
	    while (n--)
	    {
		if (buffer + size <= end)
		{
		    switch (size) {
		    case 1:
			glyph = *((CARD8 *)buffer); break;
		    case 2:
			glyph = *((CARD16 *)buffer); break;
		    case 4:
		    default:
			glyph = *((CARD32 *)buffer); break;
		    }
		    if ((*glyphs = FindGlyph (glyphSet, glyph)))
		    {
			lists->len++;
			glyphs++;
		    }
		}
		buffer += size;
	    }
	    space = size * elt->len;
	    if (space & 3)
		buffer += 4 - (space & 3);
	    lists++;
	}
    }
    if (buffer > end)
	return BadLength;

    CompositeGlyphs (stuff->op,
		     pSrc,
		     pDst,
		     pFormat,
		     stuff->xSrc,
		     stuff->ySrc,
		     nlist,
		     listsBase,
		     glyphsBase);

    if (glyphsBase != glyphsLocal)
	free(glyphsBase);
    if (listsBase != listsLocal)
	free(listsBase);
    
    return Success;
}
