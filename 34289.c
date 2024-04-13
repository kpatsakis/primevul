PanoramiXRenderFillRectangles (ClientPtr client)
{
    PanoramiXRes    *dst;
    int		    result = Success, j;
    REQUEST(xRenderFillRectanglesReq);
    char	    *extra;
    int		    extra_len;

    REQUEST_AT_LEAST_SIZE (xRenderFillRectanglesReq);
    VERIFY_XIN_PICTURE (dst, stuff->dst, client, DixWriteAccess);
    extra_len = (client->req_len << 2) - sizeof (xRenderFillRectanglesReq);
    if (extra_len &&
	(extra = (char *) malloc(extra_len)))
    {
	memcpy (extra, stuff + 1, extra_len);
	FOR_NSCREENS_FORWARD(j) {
	    if (j) memcpy (stuff + 1, extra, extra_len);
	    if (dst->u.pict.root)
	    {
		int x_off = screenInfo.screens[j]->x;
		int y_off = screenInfo.screens[j]->y;

		if(x_off || y_off) {
		    xRectangle	*rects = (xRectangle *) (stuff + 1);
		    int		i = extra_len / sizeof (xRectangle);

		    while (i--)
		    {
			rects->x -= x_off;
			rects->y -= y_off;
			rects++;
		    }
		}
	    }
	    stuff->dst = dst->info[j].id;
	    result = (*PanoramiXSaveRenderVector[X_RenderFillRectangles]) (client);
	    if(result != Success) break;
	}
	free(extra);
    }

    return result;
}
