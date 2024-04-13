PanoramiXRenderChangePicture (ClientPtr client)
{
    PanoramiXRes    *pict;
    int		    result = Success, j;
    REQUEST(xRenderChangePictureReq);

    REQUEST_AT_LEAST_SIZE(xRenderChangePictureReq);
    
    VERIFY_XIN_PICTURE(pict, stuff->picture, client, DixWriteAccess);
    
    FOR_NSCREENS_BACKWARD(j) {
        stuff->picture = pict->info[j].id;
        result = (*PanoramiXSaveRenderVector[X_RenderChangePicture]) (client);
        if(result != Success) break;
    }

    return result;
}
