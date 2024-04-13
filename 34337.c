SProcRenderCompositeGlyphs (ClientPtr client)
{
    register int n;
    xGlyphElt	*elt;
    CARD8	*buffer;
    CARD8	*end;
    int		space;
    int		i;
    int		size;
    
    REQUEST(xRenderCompositeGlyphsReq);
    
    switch (stuff->renderReqType) {
    default:			    size = 1; break;
    case X_RenderCompositeGlyphs16: size = 2; break;
    case X_RenderCompositeGlyphs32: size = 4; break;
    }
	    
    swaps(&stuff->length, n);
    swapl(&stuff->src, n);
    swapl(&stuff->dst, n);
    swapl(&stuff->maskFormat, n);
    swapl(&stuff->glyphset, n);
    swaps(&stuff->xSrc, n);
    swaps(&stuff->ySrc, n);
    buffer = (CARD8 *) (stuff + 1);
    end = (CARD8 *) stuff + (client->req_len << 2);
    while (buffer + sizeof (xGlyphElt) < end)
    {
	elt = (xGlyphElt *) buffer;
	buffer += sizeof (xGlyphElt);
	
	swaps (&elt->deltax, n);
	swaps (&elt->deltay, n);
	
	i = elt->len;
	if (i == 0xff)
	{
	    swapl (buffer, n);
	    buffer += 4;
	}
	else
	{
	    space = size * i;
	    switch (size) {
	    case 1:
		buffer += i;
		break;
	    case 2:
		while (i--)
		{
		    swaps (buffer, n);
		    buffer += 2;
		}
		break;
	    case 4:
		while (i--)
		{
		    swapl (buffer, n);
		    buffer += 4;
		}
		break;
	    }
	    if (space & 3)
		buffer += 4 - (space & 3);
	}
    }
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
