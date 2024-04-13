PanoramiXRenderSetPictureFilter (ClientPtr client)
{
    REQUEST(xRenderSetPictureFilterReq);
    int		    result = Success, j;
    PanoramiXRes    *pict;

    REQUEST_AT_LEAST_SIZE(xRenderSetPictureFilterReq);
    
    VERIFY_XIN_PICTURE(pict, stuff->picture, client, DixWriteAccess);
    
    FOR_NSCREENS_BACKWARD(j) {
        stuff->picture = pict->info[j].id;
        result = (*PanoramiXSaveRenderVector[X_RenderSetPictureFilter]) (client);
        if(result != Success) break;
    }

    return result;
}
