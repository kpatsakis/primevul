ProcRenderSetPictureClipRectangles (ClientPtr client)
{
    REQUEST(xRenderSetPictureClipRectanglesReq);
    PicturePtr	    pPicture;
    int		    nr;

    REQUEST_AT_LEAST_SIZE(xRenderSetPictureClipRectanglesReq);
    VERIFY_PICTURE (pPicture, stuff->picture, client, DixSetAttrAccess);
    if (!pPicture->pDrawable)
        return BadDrawable;

    nr = (client->req_len << 2) - sizeof(xRenderSetPictureClipRectanglesReq);
    if (nr & 4)
	return BadLength;
    nr >>= 3;
    return SetPictureClipRects (pPicture,
				  stuff->xOrigin, stuff->yOrigin,
				  nr, (xRectangle *) &stuff[1]);
}
