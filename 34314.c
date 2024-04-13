ProcRenderFillRectangles (ClientPtr client)
{
    PicturePtr	    pDst;
    int             things;
    REQUEST(xRenderFillRectanglesReq);
    
    REQUEST_AT_LEAST_SIZE (xRenderFillRectanglesReq);
    if (!PictOpValid (stuff->op))
    {
	client->errorValue = stuff->op;
	return BadValue;
    }
    VERIFY_PICTURE (pDst, stuff->dst, client, DixWriteAccess);
    if (!pDst->pDrawable)
        return BadDrawable;
    
    things = (client->req_len << 2) - sizeof(xRenderFillRectanglesReq);
    if (things & 4)
	return BadLength;
    things >>= 3;
    
    CompositeRects (stuff->op,
		    pDst,
		    &stuff->color,
		    things,
		    (xRectangle *) &stuff[1]);
    
    return Success;
}
