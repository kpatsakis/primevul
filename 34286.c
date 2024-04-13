PanoramiXRenderCreatePicture (ClientPtr client)
{
    REQUEST(xRenderCreatePictureReq);
    PanoramiXRes    *refDraw, *newPict;
    int		    result, j;

    REQUEST_AT_LEAST_SIZE(xRenderCreatePictureReq);
    result = dixLookupResourceByClass((pointer *)&refDraw, stuff->drawable,
				      XRC_DRAWABLE, client, DixWriteAccess);
    if (result != Success)
	return (result == BadValue) ? BadDrawable : result;
    if(!(newPict = (PanoramiXRes *) malloc(sizeof(PanoramiXRes))))
	return BadAlloc;
    newPict->type = XRT_PICTURE;
    newPict->info[0].id = stuff->pid;
    
    if (refDraw->type == XRT_WINDOW &&
	stuff->drawable == screenInfo.screens[0]->root->drawable.id)
    {
	newPict->u.pict.root = TRUE;
    }
    else
	newPict->u.pict.root = FALSE;

    for(j = 1; j < PanoramiXNumScreens; j++)
	newPict->info[j].id = FakeClientID(client->index);
    
    FOR_NSCREENS_BACKWARD(j) {
	stuff->pid = newPict->info[j].id;
	stuff->drawable = refDraw->info[j].id;
	result = (*PanoramiXSaveRenderVector[X_RenderCreatePicture]) (client);
	if(result != Success) break;
    }

    if (result == Success)
	AddResource(newPict->info[0].id, XRT_PICTURE, newPict);
    else 
	free(newPict);

    return result;
}
