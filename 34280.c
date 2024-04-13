PanoramiXRenderAddTraps (ClientPtr client)
{
    PanoramiXRes    *picture;
    int		    result = Success, j;
    REQUEST(xRenderAddTrapsReq);
    char	    *extra;
    int		    extra_len;
    INT16    	    x_off, y_off;

    REQUEST_AT_LEAST_SIZE (xRenderAddTrapsReq);
    VERIFY_XIN_PICTURE (picture, stuff->picture, client, DixWriteAccess);
    extra_len = (client->req_len << 2) - sizeof (xRenderAddTrapsReq);
    if (extra_len &&
	(extra = (char *) malloc(extra_len)))
    {
	memcpy (extra, stuff + 1, extra_len);
	x_off = stuff->xOff;
	y_off = stuff->yOff;
	FOR_NSCREENS_FORWARD(j) {
	    if (j) memcpy (stuff + 1, extra, extra_len);
	    stuff->picture = picture->info[j].id;
	    
	    if (picture->u.pict.root)
	    {
		stuff->xOff = x_off + screenInfo.screens[j]->x;
		stuff->yOff = y_off + screenInfo.screens[j]->y;
	    }
	    result = (*PanoramiXSaveRenderVector[X_RenderAddTraps]) (client);
	    if(result != Success) break;
	}
	free(extra);
    }

    return result;
}
