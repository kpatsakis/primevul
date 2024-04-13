ProcRenderCreatePicture (ClientPtr client)
{
    PicturePtr	    pPicture;
    DrawablePtr	    pDrawable;
    PictFormatPtr   pFormat;
    int		    len, error, rc;
    REQUEST(xRenderCreatePictureReq);

    REQUEST_AT_LEAST_SIZE(xRenderCreatePictureReq);

    LEGAL_NEW_RESOURCE(stuff->pid, client);
    rc = dixLookupDrawable(&pDrawable, stuff->drawable, client, 0,
			   DixReadAccess|DixAddAccess);
    if (rc != Success)
	return rc;

    rc = dixLookupResourceByType((pointer *)&pFormat, stuff->format,
				 PictFormatType, client, DixReadAccess);
    if (rc != Success)
	return rc;

    if (pFormat->depth != pDrawable->depth)
	return BadMatch;
    len = client->req_len - bytes_to_int32(sizeof(xRenderCreatePictureReq));
    if (Ones(stuff->mask) != len)
	return BadLength;
    
    pPicture = CreatePicture (stuff->pid,
			      pDrawable,
			      pFormat,
			      stuff->mask,
			      (XID *) (stuff + 1),
			      client,
			      &error);
    if (!pPicture)
	return error;
    if (!AddResource (stuff->pid, PictureType, (pointer)pPicture))
	return BadAlloc;
    return Success;
}
