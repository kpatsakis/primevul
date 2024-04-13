PanoramiXRenderFreePicture (ClientPtr client)
{
    PanoramiXRes *pict;
    int         result = Success, j;
    REQUEST(xRenderFreePictureReq);

    REQUEST_SIZE_MATCH(xRenderFreePictureReq);

    client->errorValue = stuff->picture;

    VERIFY_XIN_PICTURE(pict, stuff->picture, client, DixDestroyAccess);
    

    FOR_NSCREENS_BACKWARD(j) {
	stuff->picture = pict->info[j].id;
	result = (*PanoramiXSaveRenderVector[X_RenderFreePicture]) (client);
	if(result != Success) break;
    }

    /* Since ProcRenderFreePicture is using FreeResource, it will free
	our resource for us on the last pass through the loop above */
 
    return result;
}
