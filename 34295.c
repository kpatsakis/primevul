PanoramiXRenderTrapezoids(ClientPtr client)
{
    PanoramiXRes        *src, *dst;
    int                 result = Success, j;
    REQUEST(xRenderTrapezoidsReq);
    char		*extra;
    int			extra_len;
    
    REQUEST_AT_LEAST_SIZE (xRenderTrapezoidsReq);
    
    VERIFY_XIN_PICTURE (src, stuff->src, client, DixReadAccess);
    VERIFY_XIN_PICTURE (dst, stuff->dst, client, DixWriteAccess);

    extra_len = (client->req_len << 2) - sizeof (xRenderTrapezoidsReq);

    if (extra_len &&
	(extra = (char *) malloc(extra_len))) {
	memcpy (extra, stuff + 1, extra_len);

	FOR_NSCREENS_FORWARD(j) {
	    if (j) memcpy (stuff + 1, extra, extra_len);
	    if (dst->u.pict.root) {
		int x_off = screenInfo.screens[j]->x;
		int y_off = screenInfo.screens[j]->y;

		if(x_off || y_off) {
                    xTrapezoid  *trap = (xTrapezoid *) (stuff + 1);
		    int         i = extra_len / sizeof (xTrapezoid);

		    while (i--) {
			trap->top -= y_off;
			trap->bottom -= y_off;
			trap->left.p1.x -= x_off;
			trap->left.p1.y -= y_off;
			trap->left.p2.x -= x_off;
			trap->left.p2.y -= y_off;
			trap->right.p1.x -= x_off;
			trap->right.p1.y -= y_off;
			trap->right.p2.x -= x_off;
			trap->right.p2.y -= y_off;
			trap++;
		    }
		}
	    }
	    
            stuff->src = src->info[j].id;
            stuff->dst = dst->info[j].id;
	    result =
		(*PanoramiXSaveRenderVector[X_RenderTrapezoids]) (client);

	    if(result != Success) break;
	}
	
        free(extra);
    }

    return result;
}
