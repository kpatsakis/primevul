ProcRenderSetPictureTransform (ClientPtr client)
{
    REQUEST(xRenderSetPictureTransformReq);
    PicturePtr	pPicture;

    REQUEST_SIZE_MATCH(xRenderSetPictureTransformReq);
    VERIFY_PICTURE (pPicture, stuff->picture, client, DixSetAttrAccess);
    return SetPictureTransform (pPicture, (PictTransform *) &stuff->transform);
}
