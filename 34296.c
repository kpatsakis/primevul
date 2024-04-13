PanoramiXRenderTriFan(ClientPtr client)
{
    PanoramiXRes        *src, *dst;
    int                 result = Success, j;
    REQUEST(xRenderTriFanReq);
    char		*extra;
    int			extra_len;
    
    REQUEST_AT_LEAST_SIZE (xRenderTriFanReq);
    
    VERIFY_XIN_PICTURE (src, stuff->src, client, DixReadAccess);
    VERIFY_XIN_PICTURE (dst, stuff->dst, client, DixWriteAccess);

    extra_len = (client->req_len << 2) - sizeof (xRenderTriFanReq);

    if (extra_len &&
	(extra = (char *) malloc(extra_len))) {
	memcpy (extra, stuff + 1, extra_len);

	FOR_NSCREENS_FORWARD(j) {
	    if (j) memcpy (stuff + 1, extra, extra_len);
	    if (dst->u.pict.root) {
		int x_off = screenInfo.screens[j]->x;
		int y_off = screenInfo.screens[j]->y;

		if(x_off || y_off) {
                    xPointFixed  *fixed = (xPointFixed *) (stuff + 1);
		    int         i = extra_len / sizeof (xPointFixed);

		    while (i--) {
			fixed->x -= x_off;
			fixed->y -= y_off;
			fixed++;
		    }
		}
	    }
	    
            stuff->src = src->info[j].id;
            stuff->dst = dst->info[j].id;
	    result =
		(*PanoramiXSaveRenderVector[X_RenderTriFan]) (client);

	    if(result != Success) break;
	}
	
        free(extra);
    }

    return result;
}
