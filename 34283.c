PanoramiXRenderCompositeGlyphs (ClientPtr client)
{
    PanoramiXRes    *src, *dst;
    int		    result = Success, j;
    REQUEST(xRenderCompositeGlyphsReq);
    xGlyphElt	    origElt, *elt;
    INT16	    xSrc, ySrc;

    REQUEST_AT_LEAST_SIZE(xRenderCompositeGlyphsReq);
    VERIFY_XIN_PICTURE (src, stuff->src, client, DixReadAccess);
    VERIFY_XIN_PICTURE (dst, stuff->dst, client, DixWriteAccess);

    if (client->req_len << 2 >= (sizeof (xRenderCompositeGlyphsReq) +
				 sizeof (xGlyphElt)))
    {
	elt = (xGlyphElt *) (stuff + 1);
	origElt = *elt;
	xSrc = stuff->xSrc;
	ySrc = stuff->ySrc;
	FOR_NSCREENS_FORWARD(j) {
	    stuff->src = src->info[j].id;
	    if (src->u.pict.root)
	    {
		stuff->xSrc = xSrc - screenInfo.screens[j]->x;
		stuff->ySrc = ySrc - screenInfo.screens[j]->y;
	    }
	    stuff->dst = dst->info[j].id;
	    if (dst->u.pict.root)
	    {
		elt->deltax = origElt.deltax - screenInfo.screens[j]->x;
		elt->deltay = origElt.deltay - screenInfo.screens[j]->y;
	    }
	    result = (*PanoramiXSaveRenderVector[stuff->renderReqType]) (client);
	    if(result != Success) break;
	}
    }

    return result;
}
