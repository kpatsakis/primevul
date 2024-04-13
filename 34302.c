ProcRenderChangePicture (ClientPtr client)
{
    PicturePtr	    pPicture;
    REQUEST(xRenderChangePictureReq);
    int len;

    REQUEST_AT_LEAST_SIZE(xRenderChangePictureReq);
    VERIFY_PICTURE (pPicture, stuff->picture, client, DixSetAttrAccess);

    len = client->req_len - bytes_to_int32(sizeof(xRenderChangePictureReq));
    if (Ones(stuff->mask) != len)
	return BadLength;
    
    return ChangePicture (pPicture, stuff->mask, (XID *) (stuff + 1),
			  (DevUnion *) 0, client);
}
