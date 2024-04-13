ProcRenderCreateAnimCursor (ClientPtr client)
{
    REQUEST(xRenderCreateAnimCursorReq);
    CursorPtr	    *cursors;
    CARD32	    *deltas;
    CursorPtr	    pCursor;
    int		    ncursor;
    xAnimCursorElt  *elt;
    int		    i;
    int		    ret;

    REQUEST_AT_LEAST_SIZE(xRenderCreateAnimCursorReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);
    if (client->req_len & 1)
	return BadLength;
    ncursor = (client->req_len - (bytes_to_int32(sizeof(xRenderCreateAnimCursorReq)))) >> 1;
    cursors = malloc(ncursor * (sizeof (CursorPtr) + sizeof (CARD32)));
    if (!cursors)
	return BadAlloc;
    deltas = (CARD32 *) (cursors + ncursor);
    elt = (xAnimCursorElt *) (stuff + 1);
    for (i = 0; i < ncursor; i++)
    {
	ret = dixLookupResourceByType((pointer *)(cursors + i), elt->cursor,
				      RT_CURSOR, client, DixReadAccess);
	if (ret != Success)
	{
	    free(cursors);
	    return ret;
	}
	deltas[i] = elt->delay;
	elt++;
    }
    ret = AnimCursorCreate (cursors, deltas, ncursor, &pCursor, client,
			    stuff->cid);
    free(cursors);
    if (ret != Success)
	return ret;
    
    if (AddResource (stuff->cid, RT_CURSOR, (pointer)pCursor))
	return Success;
    return BadAlloc;
}
