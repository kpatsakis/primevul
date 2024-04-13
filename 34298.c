PanoramiXRenderTriangles(ClientPtr client)
{
    PanoramiXRes        *src, *dst;
    int                 result = Success, j;
    REQUEST(xRenderTrianglesReq);
    char		*extra;
    int			extra_len;
    
    REQUEST_AT_LEAST_SIZE (xRenderTrianglesReq);
    
    VERIFY_XIN_PICTURE (src, stuff->src, client, DixReadAccess);
    VERIFY_XIN_PICTURE (dst, stuff->dst, client, DixWriteAccess);

    extra_len = (client->req_len << 2) - sizeof (xRenderTrianglesReq);

    if (extra_len &&
	(extra = (char *) malloc(extra_len))) {
	memcpy (extra, stuff + 1, extra_len);

	FOR_NSCREENS_FORWARD(j) {
	    if (j) memcpy (stuff + 1, extra, extra_len);
	    if (dst->u.pict.root) {
		int x_off = screenInfo.screens[j]->x;
		int y_off = screenInfo.screens[j]->y;

		if(x_off || y_off) {
                    xTriangle  *tri = (xTriangle *) (stuff + 1);
		    int         i = extra_len / sizeof (xTriangle);

		    while (i--) {
			tri->p1.x -= x_off;
			tri->p1.y -= y_off;
			tri->p2.x -= x_off;
			tri->p2.y -= y_off;
			tri->p3.x -= x_off;
			tri->p3.y -= y_off;
			tri++;
		    }
		}
	    }
	    
            stuff->src = src->info[j].id;
            stuff->dst = dst->info[j].id;
	    result =
		(*PanoramiXSaveRenderVector[X_RenderTriangles]) (client);

	    if(result != Success) break;
	}
	
        free(extra);
    }

    return result;
}
