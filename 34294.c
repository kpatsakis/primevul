PanoramiXRenderSetPictureTransform (ClientPtr client)
{
    REQUEST(xRenderSetPictureTransformReq);
    int		    result = Success, j;
    PanoramiXRes    *pict;

    REQUEST_AT_LEAST_SIZE(xRenderSetPictureTransformReq);
    
    VERIFY_XIN_PICTURE(pict, stuff->picture, client, DixWriteAccess);
    
    FOR_NSCREENS_BACKWARD(j) {
        stuff->picture = pict->info[j].id;
        result = (*PanoramiXSaveRenderVector[X_RenderSetPictureTransform]) (client);
        if(result != Success) break;
    }

    return result;
}
