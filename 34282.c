PanoramiXRenderComposite (ClientPtr client)
{
    PanoramiXRes	*src, *msk, *dst;
    int			result = Success, j;
    xRenderCompositeReq	orig;
    REQUEST(xRenderCompositeReq);

    REQUEST_SIZE_MATCH(xRenderCompositeReq);
    
    VERIFY_XIN_PICTURE (src, stuff->src, client, DixReadAccess);
    VERIFY_XIN_ALPHA (msk, stuff->mask, client, DixReadAccess);
    VERIFY_XIN_PICTURE (dst, stuff->dst, client, DixWriteAccess);
    
    orig = *stuff;
    
    FOR_NSCREENS_FORWARD(j) {
	stuff->src = src->info[j].id;
	if (src->u.pict.root)
	{
	    stuff->xSrc = orig.xSrc - screenInfo.screens[j]->x;
	    stuff->ySrc = orig.ySrc - screenInfo.screens[j]->y;
	}
	stuff->dst = dst->info[j].id;
	if (dst->u.pict.root)
	{
	    stuff->xDst = orig.xDst - screenInfo.screens[j]->x;
	    stuff->yDst = orig.yDst - screenInfo.screens[j]->y;
	}
	if (msk)
	{
	    stuff->mask = msk->info[j].id;
	    if (msk->u.pict.root)
	    {
		stuff->xMask = orig.xMask - screenInfo.screens[j]->x;
		stuff->yMask = orig.yMask - screenInfo.screens[j]->y;
	    }
	}
	result = (*PanoramiXSaveRenderVector[X_RenderComposite]) (client);
	if(result != Success) break;
    }

    return result;
}
