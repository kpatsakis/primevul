ProcRenderQueryPictIndexValues (ClientPtr client)
{
    PictFormatPtr   pFormat;
    int		    rc, num;
    int		    rlength;
    int		    i, n;
    REQUEST(xRenderQueryPictIndexValuesReq);
    xRenderQueryPictIndexValuesReply *reply;
    xIndexValue	    *values;

    REQUEST_AT_LEAST_SIZE(xRenderQueryPictIndexValuesReq);

    rc = dixLookupResourceByType((pointer *)&pFormat, stuff->format,
				 PictFormatType, client, DixReadAccess);
    if (rc != Success)
	return rc;

    if (pFormat->type != PictTypeIndexed)
    {
	client->errorValue = stuff->format;
	return BadMatch;
    }
    num = pFormat->index.nvalues;
    rlength = (sizeof (xRenderQueryPictIndexValuesReply) + 
	       num * sizeof(xIndexValue));
    reply = (xRenderQueryPictIndexValuesReply *) malloc(rlength);
    if (!reply)
	return BadAlloc;

    reply->type = X_Reply;
    reply->sequenceNumber = client->sequence;
    reply->length = bytes_to_int32(rlength - sizeof(xGenericReply));
    reply->numIndexValues = num;

    values = (xIndexValue *) (reply + 1);
    
    memcpy (reply + 1, pFormat->index.pValues, num * sizeof (xIndexValue));
    
    if (client->swapped)
    {
	for (i = 0; i < num; i++)
	{
	    swapl (&values[i].pixel, n);
	    swaps (&values[i].red, n);
	    swaps (&values[i].green, n);
	    swaps (&values[i].blue, n);
	    swaps (&values[i].alpha, n);
	}
	swaps (&reply->sequenceNumber, n);
	swapl (&reply->length, n);
	swapl (&reply->numIndexValues, n);
    }

    WriteToClient(client, rlength, (char *) reply);
    free(reply);
    return Success;
}
