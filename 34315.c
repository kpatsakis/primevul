ProcRenderFreeGlyphSet (ClientPtr client)
{
    GlyphSetPtr     glyphSet;
    int rc;
    REQUEST(xRenderFreeGlyphSetReq);

    REQUEST_SIZE_MATCH(xRenderFreeGlyphSetReq);
    rc = dixLookupResourceByType((pointer *)&glyphSet, stuff->glyphset, GlyphSetType,
			   client, DixDestroyAccess);
    if (rc != Success)
    {
	client->errorValue = stuff->glyphset;
	return rc;
    }
    FreeResource (stuff->glyphset, RT_NONE);
    return Success;
}
