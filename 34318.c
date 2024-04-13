ProcRenderQueryFilters (ClientPtr client)
{
    REQUEST (xRenderQueryFiltersReq);
    DrawablePtr			pDrawable;
    xRenderQueryFiltersReply	*reply;
    int				nbytesName;
    int				nnames;
    ScreenPtr			pScreen;
    PictureScreenPtr		ps;
    int				i, j, len, total_bytes, rc;
    INT16			*aliases;
    char			*names;

    REQUEST_SIZE_MATCH(xRenderQueryFiltersReq);
    rc = dixLookupDrawable(&pDrawable, stuff->drawable, client, 0,
			   DixGetAttrAccess);
    if (rc != Success)
	return rc;
    
    pScreen = pDrawable->pScreen;
    nbytesName = 0;
    nnames = 0;
    ps = GetPictureScreenIfSet(pScreen);
    if (ps)
    {
	for (i = 0; i < ps->nfilters; i++)
	    nbytesName += 1 + strlen (ps->filters[i].name);
	for (i = 0; i < ps->nfilterAliases; i++)
	    nbytesName += 1 + strlen (ps->filterAliases[i].alias);
	nnames = ps->nfilters + ps->nfilterAliases;
    }
    len = ((nnames + 1) >> 1) + bytes_to_int32(nbytesName);
    total_bytes = sizeof (xRenderQueryFiltersReply) + (len << 2);
    reply = (xRenderQueryFiltersReply *) malloc(total_bytes);
    if (!reply)
	return BadAlloc;
    aliases = (INT16 *) (reply + 1);
    names = (char *) (aliases + ((nnames + 1) & ~1));
    
    reply->type = X_Reply;
    reply->sequenceNumber = client->sequence;
    reply->length = len;
    reply->numAliases = nnames;
    reply->numFilters = nnames;
    if (ps)
    {

	/* fill in alias values */
	for (i = 0; i < ps->nfilters; i++)
	    aliases[i] = FilterAliasNone;
	for (i = 0; i < ps->nfilterAliases; i++)
	{
	    for (j = 0; j < ps->nfilters; j++)
		if (ps->filterAliases[i].filter_id == ps->filters[j].id)
		    break;
	    if (j == ps->nfilters)
	    {
		for (j = 0; j < ps->nfilterAliases; j++)
		    if (ps->filterAliases[i].filter_id == 
			ps->filterAliases[j].alias_id)
		    {
			break;
		    }
		if (j == ps->nfilterAliases)
		    j = FilterAliasNone;
		else
		    j = j + ps->nfilters;
	    }
	    aliases[i + ps->nfilters] = j;
	}

	/* fill in filter names */
	for (i = 0; i < ps->nfilters; i++)
	{
	    j = strlen (ps->filters[i].name);
	    *names++ = j;
	    strncpy (names, ps->filters[i].name, j);
	    names += j;
	}
	
	/* fill in filter alias names */
	for (i = 0; i < ps->nfilterAliases; i++)
	{
	    j = strlen (ps->filterAliases[i].alias);
	    *names++ = j;
	    strncpy (names, ps->filterAliases[i].alias, j);
	    names += j;
	}
    }

    if (client->swapped)
    {
	register int n;

	for (i = 0; i < reply->numAliases; i++)
	{
	    swaps (&aliases[i], n);
	}
    	swaps(&reply->sequenceNumber, n);
    	swapl(&reply->length, n);
	swapl(&reply->numAliases, n);
	swapl(&reply->numFilters, n);
    }
    WriteToClient(client, total_bytes, (char *) reply);
    free(reply);
    
    return Success;
}
